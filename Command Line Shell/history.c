/**
 * @file 
 * Creates the history for what the user inputs, up to 100 commands.
 * Written by Eric Chen
 * */
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "history.h"


/** -- Initializing the max_commands, current_command, and command_history -- */
unsigned int max_commands;
unsigned int current_command;
char* * command_history;

/**
 *  Function to create the command_history.
 *  @param limit: the limit of how many commands the history will hold.
 *  @return: void
 * */
void hist_init(unsigned int limit)
{
    // TODO: set up history data structures, with 'limit' being the maximum
    // number of entries maintained.
    max_commands = limit;
    command_history = (char**) malloc(max_commands*sizeof(char*));
    for (int i = 0; i < max_commands; i++)
    {
        command_history[i] = NULL; 
    }
}
/**
 *  Function to return the command_history.
 *  @param index: the index of where we are at in the command_history to implement arrow key up and down usability.
 *  @return the command that we're on or NULL if none.
 * */
char* get_history(int index) {
    if (index >= current_command) {
        return NULL;
    }
    else {
        return command_history[index];
    }
    return NULL;
}

/**
 *  Function to return the current_command.
 * * @param: none
 * * @return: the command that we're on.
 * */
int get_current_command() {
    return current_command;
}

/**
 *   Function to free the command_history
 * * @param: void
 * * @return: void
 * */
void hist_destroy(void)
{
    for (int i = 0; i < max_commands; i++)
    {
        if (command_history[i] != NULL)
        {
            free(command_history[i]);
        }
    }
    free(command_history);
}

/**
 *   Function to add current commands to the command history
 *  @param cmd: the command that we're adding to the command history
 *  @return void
 * */
void hist_add(const char *cmd)
{
    command_history[current_command % max_commands] = (char*) cmd;
    current_command++;
}

/**
 *  Function to print our command history.
 *  @param void
 *  @return void
 * */
void hist_print(void)
{
    for (int i = current_command % max_commands; i < max_commands; i++) { //calculate start index
        if (command_history[i] != NULL) {
            printf("%u %s\n", i+(((current_command - 100) / max_commands) * 100) +1, command_history[i]);
        }
    }

    for (int i = 0; i != current_command % max_commands; i++) {  //loop through
        printf("%u %s\n", i + ((current_command / max_commands) * 100) + 1, command_history[i]);
    }
    fflush(stdout);
}

/**
 *   Function to search through our list using the prefix that the user inputs after a ! and executes the command.
 * @param prefix: the prefix that we're using to look for the command.
 * @return the command that we're looking for, NULL otherwise.
 * */

const char *hist_search_prefix(char *prefix)
{
    // TODO: Retrieves the most recent command starting with 'prefix', or NULL
    // if no match found.
    int i = 0;

    if (prefix == NULL || strlen(prefix) == 0) {
        return NULL;
    }

    // getting rid of the exclamation point
    for (i = 0; i < strlen(prefix)-1; i++) {
        prefix[i] = prefix[i+1];
    }

    prefix[i] = 0;  //sets null byte at the end

    for (i = (current_command % max_commands) - 1; i >= 0; i--) {
        if (strlen(prefix) > strlen(command_history[i])) {
            break;
        }
        if (strncmp(command_history[i], prefix, strlen(prefix)) == 0) {
            return command_history[i];
        }
        else {
            continue;
        }
    }

    for (i = max_commands - 1; i >= (current_command % max_commands); i--) { //going through the remainder
        if (strncmp(command_history[i], prefix, strlen(prefix)) == 0) {
            return command_history[i];
        }
    }
    return NULL;
}

/**
 *   Function to search and execute a command using a number. For example, if ls were #2 on our list and we ran !2, it should run ls.
 * @param command_number: the command that is in the number that the user inputs after the !.
 * @return  the command that we're looking for, NULL otherwise.
 * */
const char *hist_search_cnum(int command_number)
{
    if (command_number >= current_command)
    {
        return NULL;
    }
    else if (command_number < 0)
    {
        return NULL;
    }
    else if (command_number < current_command - max_commands)
    {
        return NULL;
    }
    else
    {
        return command_history[command_number % max_commands];
    }
}

/**
 * Function for !! to execute previous command.
 * @param command_number: the command that is numbered in command_history.
 * @return the command that we're looking for, NULL otherwise
 * */
const char *hist_double_bang(int command_number) {
    if (command_number >= current_command) {
        return NULL;
    }
    else if (command_number < 0) {
        return NULL;
    }
    else {
        return command_history[command_number % max_commands];
    }
}

/**
 *   Function that retrieves the most recent command number
 * @param: void
 * @return: the most recent command number
 * */
unsigned int hist_last_cnum(void)
{
    // TODO: Retrieve the most recent command number.
    return current_command;
}