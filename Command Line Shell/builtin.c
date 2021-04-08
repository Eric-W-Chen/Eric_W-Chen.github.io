/**
 * @file 
 * Builtin implementations. 
 * Written by Eric Chen
 * */
#include "builtin.h"
#include "stdlib.h"
#include <pwd.h>
#include <stddef.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include "logger.h"
#include "history.h"

/** -- Private function forward declarations -- */
int change_dir(int argc, char *argv[]);
int exit_shell(int argc, char *argv[]);
int history_execution(int argc, char *argv[]);

/** Creates the builtin struct that will execute a built-in function depending on what the user inputs */
struct builtin_def builtins[] = {
	{ "cd", change_dir },
	{ "exit", exit_shell },
	{ "history", history_execution},
};

/**
 *   Sets up the cd built-in command. If no arguments, it will return to the user's home directory.
 * * @param: int argc: argument count
 * * @param: int *argv[]: argument vector
 * * @return: home directory if no arguments, otherwise change directory depending on user arguments.
 * */
int change_dir(int argc, char *argv[]){
	if (argc == 1) {
		char home[100] = "/home/";
		strcat(home, getlogin());
		return chdir(home);
	}
	else {
		return chdir(argv[1]);
	}
}

/**
 *  Sets up the exit built-in command. 
 * * @param: int argc: argument count
 * * @param: int *argv[]: argument vector
 * * @return: -2 to execute the exit
 * */
int exit_shell(int argc, char *argv[]){
	exit(0);
	return -2;
}

/**
 *   Prints out the history list if user inputs "history".
 * * @param: int argc: argument count
 * * @param: int *argv[]: argument vector
 * * @return: 0 when successful.
 * */
int history_execution(int argc, char *argv[]) {
	hist_print();
	return 0;
}

/**
 *   Checks to see if argument is an empty string, a !, or a builtin.
 * * @param: int argc: argument count
 * * @param: int *argv[]: argument vector
 * * @return: 1 when successful.
 * */
int handle_builtin(int argc, char *argv[]){
	if (argv[0] == NULL){
		LOGP("Arg is NULL. Not a built in.\n");
		return 1;
	}

	if (argv[0][0] == '!'){
		return history_execution(argc, argv);
	}	

	for (int i=0; i<sizeof(builtins) / sizeof(struct builtin_def); ++i){
		if (strcmp(argv[0], builtins[i].name) == 0){
			return builtins[i].function(argc, argv);
		}
	}
	return 1;
}
