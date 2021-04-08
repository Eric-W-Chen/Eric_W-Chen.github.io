/**
 * @file 
 * Creates the shell that can read commands and execute them while keeping track of history.
 * Written by Eric Chen
 * */

#include <fcntl.h>
#include <pwd.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/param.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <ctype.h>

#include "history.h"
#include "logger.h"
#include "ui.h"
#include "builtin.h"

/** Creates the command line struct that will hold the token, the standard out pipe, and the standard out file for piping */
typedef struct command_line {
    char **tokens;
    bool stdout_pipe;
    char *stdout_file;
} command_line;

/** Creates the command line struct that will hold the pid of a background job, and the command, to hold background jobs */
typedef struct bg_job {
    char command[100];
    pid_t pid;
} bg_job;


/** -- Initializing the job_history, command, and child -- */
char* * job_history;
char *command;
pid_t child;

/** Initializing struct for background jobs */
struct bg_job job [10] = { 0 };

/**
 * Sets up a pipeline piece by piece.
 * @param cmds: the command line struct
 * @return -1 if unsuccessful, 0 if successful
 * */
int execute_pipeline(command_line *cmds){ 
    int fd[2];
    int iterator = 0;

    int open_flags = O_RDWR | O_CREAT | O_TRUNC;

    while (cmds[iterator].stdout_pipe){
        if (pipe(fd) == -1) {
            return -1;
        }

        pid_t pid = fork();

        if (pid == -1) {
            return -1;
        }

        if (pid == 0) {
            dup2(fd[1], STDOUT_FILENO);
            close(fd[0]);
            if (execvp(cmds[iterator].tokens[0], cmds[iterator].tokens) == -1) {
                close(fd[0]);
                close(fd[1]);
                close(STDOUT_FILENO);
                close(STDIN_FILENO);
            }
        }

        else {
            if (dup2(fd[0], STDIN_FILENO) == -1) {
                return -1;
            }
            close(fd[1]);
        }
        iterator++;
    }


    if (cmds[iterator].stdout_file != NULL){
        int open_perms = 0644;
        int fd2 = open(cmds[iterator].stdout_file, open_flags, open_perms);

        if (dup2(fd2, STDOUT_FILENO) == -1) {
            perror("dup2");
            return -1;
        }
        close(fd2);
    }

    if (execvp(cmds[iterator].tokens[0], cmds[iterator].tokens) == -1) {
        perror("execvp");
        return -1;
    }

    return 0;
}

/**
 *  Checks if the command is a pipe or redirect.
 * @param command: the command we're checking
 * @return true if it's a pipe or redirect, false otherwise
 * */
bool is_pipe(char *command) { 
    if (strstr(command, " | ") != NULL || strstr(command, " > ") != NULL) {
        return true;
    }
    return false;
}

/**
 *   Tokenizing the pipe or redirect.
 *   @param command: so we can check the command we're tokenizing
 *   @param num_pipes: how many tokens we're putting in
 * * @return void
 * */
void pipe_tokenize(char **command, int num_pipes) {
    command_line cmds[num_pipes];

    int status; 
    int index = 0;

    cmds[index].tokens = &command[0];
    cmds[index].stdout_pipe = true;
    cmds[index].stdout_file = NULL;

    index++;

    for (int i = 0; i < num_pipes; i++){
        if (strcmp(command[i], "|") == 0) {
            command[i] = NULL;
            i++;
            cmds[index].tokens = &command[i];
            cmds[index].stdout_pipe = true;
            cmds[index].stdout_file = NULL;
            index++;
        }
        else if (strcmp(command[i], ">") == 0) {
            command[i] = NULL;
            i++;
            cmds[index-1].stdout_file = command[i];
            break;
        }
    }

    index--;
    cmds[index].stdout_pipe = false;

    pid_t pipe_child = fork();

    if (pipe_child == 0) {
        execute_pipeline(cmds);
    }
    else if (pipe_child == -1) {
        perror("fork");
    }
    else {
        waitpid(pipe_child, &status, 0);
    }
}

/**
 *  Handler for when user sends a sigint (Ctrl+C) so that the shell does not quit.
 * @param signo: the signal number we're sending
 * @return void
 * */
void sigint_handler(int signo) {
    // If the user presses CTRL-C while a child process is running (e.g sleep)
    if (child != 0)     //it's a child process
    {  
        kill(child, SIGINT); //shell kills current child
    }

    fflush(stdout);
    return;
}

/**
 *  Handler for when there's a background job and we check to see which jobs in our job list are background jobs.
 * @param signo: the signal number we're sending
 * @return void
 * */
void sigchild_handler(int signo) {
    int return_status = waitpid(-1, &return_status, WNOHANG);
    for (int i = 0; i < 10; i++){
        if (return_status == job[i].pid) {
            job[i].pid = 0;
        }
    }
}

/**
 *  Adding background jobs into our job list 
 * @param cmd: command we're running
 * @param pid: pid so we can see if the job is background job or not
 * @return void
 * */
void jobs_add(char *cmd, pid_t pid) {
    for (int i = 0 ; i < 10; i++) {
        if (job[i].pid == 0) {
            job[i].pid = pid; //pid of child process
            strcpy(job[i].command, cmd);
            break;  //successfully added
        }
    }
}

/**
 *  Printing our background jobs 
 * @param void
 * @return void
 * */
void job_print(void) {
    int j = 0;
    for (int i = 0; i < 10; i++){
        if (job[i].pid != 0) {   //when pid is 0 the history isn't there anymore
            printf("%d %s \n", j, job[i].command);
            j++;
        }
    }
}

/**
 *  Main function so we can initialize our history, ui, and read and execute the commands that the user inputs or scripts. 
 * @param void
 * @return: 0 if successful -1 if failure.
 * */
int main(void)
{
    init_ui();
    hist_init(100);

    signal(SIGINT, sigint_handler);

    while (true) {
        fflush(stdout);

        command = read_command();
        if (command == NULL) {
            break;
        }

        char * command_copy = strdup(command);

        if (strlen(command_copy) >= 2 && command_copy[0] == '!' && command_copy[1] == '!') {
            char* hist_ptr = (char*) hist_double_bang(hist_last_cnum()-1);
            if (hist_ptr == NULL) {
                continue;
            }
            free(command_copy);
            free(command);
            command_copy = strdup(hist_ptr);
            command = strdup(command_copy);
        }
        else if (strlen(command_copy) >= 2 && command_copy[0] == '!' && isdigit(command_copy[1])) {
            int command_num;
            sscanf(command_copy, "!%d", &command_num);
            char *hist_command_ptr = (char*) hist_search_cnum(command_num-1);
            if (hist_command_ptr == NULL) {
                continue;
            }
            else {
                free(command_copy);
                free(command);
                command_copy = strdup(hist_command_ptr);
                command = strdup(command_copy);
            }
        }   
        else if (strlen(command_copy) >= 2 && command_copy[0] == '!' && isalpha(command_copy[1])) {
            char *prefix_command_ptr = (char*) hist_search_prefix(command_copy);
            if (prefix_command_ptr == NULL) {
                continue;
            }
            else {
                free(command_copy);
                free(command);
                command_copy = strdup(prefix_command_ptr);
                command = strdup(command_copy);
            }
        } 

        hist_add(command_copy);

        char *args[1000] = { 0 };
        int tokens = 0;
        char *next_tok = command;
        char *curr_tok;
        bool background = false;

        if (strstr(command, "&") != NULL) {
            background = true;
        }
        else {
            background = false;
        }

        if (!strcmp(command, "jobs")) {
            job_print();
            continue;
        }

        char *comm_pt_backup = command;

        while ((curr_tok = next_token(&next_tok, " \t")) != NULL) {
            if (curr_tok[0] == '&') {   //replacing & with null so execvp knows when to stop
                continue; //more stuff after & sign
            }

            if (curr_tok[0] == '#') {
                break;
            }
            args[tokens++] = curr_tok;
        }

        args[tokens] = NULL;

        if (is_pipe(command_copy)){
            pipe_tokenize(args, tokens);
            free(comm_pt_backup);
            continue;
        }

        if (handle_builtin(tokens, args) != 0){
            child = fork();
            if (child == 0){  //if we're the child
                int ret = execvp(args[0], args);
                if (ret == -1){
                    perror("execvp");
                }
                fclose(stdin);
                return EXIT_FAILURE;
            }
            else if (child == -1) {   //error
                perror("execvp");
            }
            else if (!background) {    //we're the parent
                int return_status;
                waitpid(child, &return_status, 0); //wait for child process to finish if it's not a bg process

            }
            else {                                     //if there is a background
                signal(SIGCHLD, sigchild_handler); //set sig child handler on sig child
                jobs_add(command_copy, child);   //add it to our jobs list with the child id
                //free(command_p);
            }
        }
        free(comm_pt_backup);
    }

    hist_destroy();
    return 0;
}   