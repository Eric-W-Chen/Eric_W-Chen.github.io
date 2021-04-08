# Command Line Shell

Author: Eric Chen

## About This Project
In this project, we implemented a Shell that has a prompt, ui, and general interactive functionality. Features of this shell include a prompt that will include the command number (starting from 1), user name and host name, the current working directory, and the process exit status (indicated from an emoji: smiling face for success (exit code 0) and a sick face for failure (any nonzero exit code or failure to execute the child proces), scripting which reads commands from standard input and executes them without showing the prompt, built-in commands which include cd and exit, signal handling where the shell gracefully handles the user pressing Ctrl+C, history where only the last 100 are kept, redirection where the shell supports file output redirection and pip redirection, background jobs where if a command ends in &, then it runs in the background, incremental history search where when the user presses the 'up' arrow key on the keyboard it displays the most recent history entry and if the user presses 'down' they navigate back toward the most recently-entered command in history, ending back at the start, and tab completion where when the user presses the tab key, it autocompletes their command. 

## What is a Shell?
The outermost layer of the oeprating system is called the shell. In Unix-based systems, the shell is generally a command line interface. Most Linux distributions ship with bash as the default. It gathers input from you and eecutes programs based on that input and when a program finishes executing, it displays that program's output.

Source(s): https://www.tutorialspoint.com/unix/unix-what-is-shell.htm

##Program Options
Unix tradition encourages the use of command-line switches to control programs, so that options can be specified from scripts. In the original Unix tradiiton, command-line options are single letters preceded by a single hyphen.

Please refer to this when looking for command-line options: http://www.catb.org/~esr/writings/taoup/html/ch10s05.html 

##Included Files
There are several files included. These are:
	- <b>Makefile</b>: Included to compile and run the program.
	- <b>builtin.c</b>: To run built-in commands
	- <b>shell.c</b>: Our shell
	- <b>history.c</b>: History of commands
	- <b>ui.c</b>: User interface of our prompt for our shell.

There are also header files for each of these files.

To compile and run:

```bash
make
./crash
```
### Program Output
```bash
$ ./crash

.c:48:init_ui(): Initializing UI...
ui.c:52:init_ui(): Setting locale: en_US.UTF-8
ui.c:56:init_ui(): stdin is a TTY; entering interactive mode
[🙂]-[0]-[Erics-VM@:~/home/echen10/P2-Eric-W-Chen] ~/P2-Eric-W-Chen
╰─╼
```

##Testing

To execute the test cases, use `make test`. To pull in updated test cases, run `make testupdate`. You can also run a specific test case instead of all of them:

```
# Run all test cases:
make test

# Run a specific test case: 
make test run=4

# Run a few specific test cases (4, 8, and 12 in this case):
make test run='4 8 12'
```
./crash
```

## Testing
To execute the test cases, use `make test`. To pull in updated test cases, run `make testupdate`. You can also run a specific test case instead of all of them:

```
# Run all test cases:
make test

# Run a specific test case: 
make test run=4

# Run a few specific test cases (4, 8, and 12 in this case):
make test run='4 8 12'
```

