/**
 * @file 
 * Creates the prompt that the user sees when running the shell.
 * Written by Eric Chen
 * */

#include <stdio.h>
#include <unistd.h>
#include <readline/readline.h>
#include <locale.h>
#include <stdlib.h>
#include <stdbool.h>
#include <limits.h>
#include <pwd.h>
#include <sys/types.h>
#include <dirent.h>

#include "history.h"
#include "logger.h"
#include "ui.h"

/** -- Private function forward declarations -- */
static int readline_init(void);
int key_up(int count, int key);
int key_down(int count, int key);
static char **command_completion(const char *text, int start, int end);
char *command_generator(const char *text, int state);

static bool scripting = false;

static char prompt_str1[80] = "";
static char prompt_str2[80] = "╰─╼ ";

static char path[PATH_MAX];
static char home_dir[PATH_MAX];
static char cwd[PATH_MAX];
static int count = 0;
static int ind = 0;
static char status[5] = {0xF0, 0x9F, 0x99, 0x82};

/**
 * Checks to see whether we're in scripting mode or not.
 * * @param void
 * * @return void
 * */
void init_ui(void)
{
    LOGP("Initializing UI...\n");

    char *locale = setlocale(LC_ALL, "en_US.UTF-8");

    LOG("Setting locale: %s\n",
            (locale != NULL) ? locale : "could not set locale!");

    if (isatty(STDIN_FILENO)) {        
        LOGP("stdin is a TTY; entering interactive mode\n");        
        scripting = false;   
    } 

    else {        
        LOGP("data piped in on stdin; entering script mode\n");        
        scripting = true;   
    }

    rl_startup_hook = readline_init;
}

/**
 * Checks to see what the current directory is and replaces it with ~/ if we are in home.
 * * @param cwd: where we are storing current working directory
 * * @return the current working directory
 * */
char* current_dir(char *cwd) {
    char *home_user = getpwuid(getuid()) -> pw_dir; 
    char *dir = strstr(cwd, home_user);

    if (dir == NULL) {
        return cwd;
    }

    if (strncmp(cwd, home_user, strlen(home_user)) == 0) {
        dir = dir + strlen(home_user);
        char *home = (char*) malloc (strlen(cwd));
        strcpy(home, "~");
        strcat(home, dir);
        return home;
    }
    return dir;
}

/**
 * Creates the prompt line string with the necessary information.
 * * @param void
 * * @return char * the prompt line
 * */
char* prompt_line1(void) {
    char host[256];

    getcwd(cwd, sizeof(cwd));

    if (strncmp(cwd, home_dir, strlen(home_dir)) == 0) {
        strcpy(path, "~");
        strcat(path, &cwd[strlen(home_dir)]);
    }

    if (gethostname(host, sizeof(host)) == -1) {
        perror("hostname error");
    }

    if (getcwd(cwd, sizeof(cwd)) == NULL) {
        perror("cwd error");
    }

    strcat(prompt_str1, "╰─╼ ");
    sprintf(prompt_str1, "[%s]-[%d]-", status, count);
    sprintf(prompt_str1 + strlen(prompt_str1), "[%s@:%s] %s", host, path, current_dir(cwd));
    return prompt_str1;
}

/**
 * The arrow string before the user inputs command.
 * * @param void
 * * @return char * the arrow string
 * */
char *prompt_line2(void) {
    return prompt_str2;
}

/**
 *  If scripting is true, it reads the scrips otherwise it enters terminal mode and prints the prompt.
 * * @param void
 * * @return char * the prompt line or the script depending what mode we're on
 * */
char *read_command(void)
{
    if (scripting == true) {
        char *line = NULL;
        size_t line_sz = 0;
        ssize_t nread = getline(&line, &line_sz, stdin);
        if (nread == -1){
            LOGP("Reached end of input stream.\n");
            free(line);
            return NULL;
        }
        size_t newline = strcspn(line, "\n");
        line[newline] = '\0';
        return line;
    }
    else {
        puts(prompt_line1());
        return readline(prompt_line2());
    }
}

/**
 * Checks to see if the user is pressing arrow up, down, or tab to enable history search or autocomplete.
 * * @param void
 * * @return 0 if successful.
 * */
int readline_init(void)
{
    rl_bind_keyseq("\\e[A", key_up);
    rl_bind_keyseq("\\e[B", key_down);
    rl_bind_key('\t', rl_complete);
    rl_variable_bind("show-all-if-ambiguous", "on");
    rl_variable_bind("colored-completion-prefix", "on");
    rl_attempted_completion_function = command_completion;
    count++;
    return 0;
}

/** Retrieves the next token from a string.
 *
 * Parameters:
 * - str_ptr: maintains context in the string, i.e., where the next token in the
 *   string will be. If the function returns token N, then str_ptr will be
 *   updated to point to token N+1. To initialize, declare a char * that points
 *   to the string being tokenized. The pointer will be updated after each
 *   successive call to next_token.
 *
 * - delim: the set of characters to use as delimiters
 *
 * Returns: char pointer to the next token in the string.
 */
char *next_token(char **str_ptr, const char *delim)
{
    if (*str_ptr == NULL) {
        return NULL;
    }

    size_t tok_start = strspn(*str_ptr, delim);
    size_t tok_end = strcspn(*str_ptr + tok_start, delim);

    /* Zero length token. We must be finished. */
    if (tok_end  == 0) {
        *str_ptr = NULL;
        return NULL;
    }

    /* Take note of the start of the current token. We'll return it later. */
    char *current_ptr = *str_ptr + tok_start;

    /* Shift pointer forward (to the end of the current token) */
    *str_ptr += tok_start + tok_end;

    if (**str_ptr == '\0') {
        /* If the end of the current token is also the end of the string, we
         * must be at the last token. */
        *str_ptr = NULL;
    } else {
        /* Replace the matching delimiter with a NUL character to terminate the
         * token string. */
        **str_ptr = '\0';

        /* Shift forward one character over the newly-placed NUL so that
         * next_pointer now points at the first character of the next token. */
        (*str_ptr)++;
    }

    return current_ptr;
}

/**
 *  If the user presses the up arrow key, it goes back to the most recent command in history.
 * * @param count: checks to see how far back to go depending on how many times the user presses the up key
 * * @param key: checks to see what key is pressed
 * * @return  0 if successful
 * */
int key_up(int count, int key)
{
    ind++;
    int rev_history = get_current_command() - ind; //go back

    char* history = get_history(rev_history); //get the history before

    if (history != NULL) {
        /* Modify the command entry text: */
        rl_replace_line(history, 1);
        /* Move the cursor to the end of the line: */
        rl_point = rl_end;
    }

    return 0;
}

/**
 *  If the user presses the down  arrow key, it goes forward to the most recent command in history.
 * * @param count: checks to see how far back to go depending on how many times the user presses the up key
 * * @param key: checks to see what key is pressed
 * * @return 0 if successful
 * */
int key_down(int count, int key)
{
    ind--;
    int rev_history = get_current_command() - ind; //go forward

    char* history = get_history(rev_history); //get the history ahead

    if (ind < 0) {
        /* If we're at the last command */
        rl_replace_line("", 1);
    }

    if (history != NULL) {
        /* Modify the command entry text: */
        rl_replace_line(history, 1);
        /* Move the cursor to the end of the line: */
        rl_point = rl_end;
    }

    return 0;
}

/**
 *  Checks if we find a suitable completion.
 * * @param text: string that we're going through to look for a suitable completion
 * * @param start: where we're starting
 * * @param end: where we're ending
 * * @return if we can't find a suitable completion, returns built-in filename completion. Otherwise, the completion.
 * */
char **command_completion(const char *text, int start, int end)
{
    /* Tell readline that if we don't find a suitable completion, it should fall
     * back on its built-in filename completion. */
    rl_attempted_completion_over = 0;

    return rl_completion_matches(text, command_generator);
}

//static variables used only by command_generator
char* match_string = NULL;
size_t match_string_size = 0;
size_t match_target_idx = 0; //index of entry after last match
size_t match_target_size = 0;
char** match_target = NULL;

/**
 * This function is called repeatedly by the readline library to build a list of
 * possible completions. It returns one match per function call. Once there are
 * no more completions available, it returns NULL.
 */
char *command_generator(const char *text, int state)
{
    // Initialized when state is 0
    if (state == 0) {
        //copy match string
        if (match_string != NULL) {
            free(match_string);
            match_string = NULL;
        }
        match_string_size = strlen(text);
        match_string = malloc((match_string_size + 1));
        strncpy(match_string, text, match_string_size);
        match_string[match_string_size] = '\0';

        //set match_target = list of folder entries
        if (match_target != NULL) {
            free(match_target);
            match_target = NULL;
        }
        char* curr_tok;
        struct dirent *dir_ptr;
        char *open = getenv("PATH");

        //tokenizing function 
        while ((curr_tok = next_token(&open, " \t")) != NULL) {
            DIR *dir;
            dir = opendir(open);
            if (dir == NULL) {
                perror("error");
                return NULL;
            }
            match_target_size = 0;
            while ((dir_ptr = readdir(dir)) != NULL) {
                match_target_size++;
            }
            match_target = calloc(match_target_size, sizeof(char*));
            match_target_idx = 0;
            while ((dir_ptr = readdir(dir)) != NULL) {
                printf("%s\n", dir_ptr->d_name);
                match_target[match_target_idx] = dir_ptr->d_name;
                match_target_idx++;
            }

            closedir(dir);
        }

        //LOG("match target indx: %ld", match_target_idx);
        match_target_idx = 0;
    }       //find the next match
    while (match_target_size > 0 && match_target_idx < match_target_size
            && strncmp(match_string, match_target[match_target_idx], match_string_size) != 0) {
        //LOG("match_string: %s match_target[ match_target_idx]: %s \n", match_string, match_target[match_target_idx]);
        match_target_idx++;
    }
    if (match_target_idx >= match_target_size || match_target_idx == 0) {
        return NULL;
    }   
    return strdup(match_target[match_target_idx++]);
}