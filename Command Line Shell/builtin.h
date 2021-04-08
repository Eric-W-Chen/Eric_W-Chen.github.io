/**
 * @file
 * Functions that implement the built-in shell commands such as 'cd', 'exit',
 * etc.
 */

#ifndef _BUILTIN_H_
#define _BUILTIN_H_

/** The structure for builtin commands */
struct builtin_def {
	char name[128];
	int (*function)(int argc, char *argv[]);
};

/** Function to run builtin commands */
int handle_builtin(int argc, char *argv[]);

#endif