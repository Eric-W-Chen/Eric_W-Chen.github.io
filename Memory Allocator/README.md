# Project 3: Memory Allocator

Author: Eric Chen and Bae Sung Kim

See: https://www.cs.usfca.edu/~mmalensek/cs326/assignments/project-3.html 

## About This Project
In this project, we developed a custom memory allocator. malloc is a library function that uses system calls to allocate and deallocate memory. In most implementations of malloc, these system calls involve at least either sbrk or mmap. To simplify our allocator, we will only be using mmap and we'll allocate entire regions of memory at a time. The size of each region is a multiple of the system page size, which means that if a program executes malloc(1) for a single byte of memory and our machine has a page size of 4096, we'll allocate a region of 4096 bytes instead. Consequently, if the user requests 4097 bytes then we will allocate two regions' worth of memory.

## What is a Memory Allocator?
 Memory allocation is a process by which computer programs and services are assigned with physical or virtual memory space. It is the process of reserving a partial or complete portion of computer memory for the execution of programs and processes. Memory allocation is achieved through a process known as memory management.

Source(s): https://www.techopedia.com/definition/27492/memory-allocation

## Program Options
The memory is managed through inputs of test allocations. In this case, the use of test cases which is run by the command make test. 

## Included Files
There are several files included. These are:
	- <b>Makefile</b>: Included to compile and run the program.
	- <b>allocator.c</b>: The memory allocator.

There are also header files for each of these files.

To compile and use the allocator:

```bash
make
LD_PRELOAD=$(pwd)/allocator.so ls /
```

(in this example, the command `ls /` is run with the custom memory allocator instead of the default).

### Program Output
```bash
The program cannot be run interactively through ./allocator and requires inputs of test allocations to test the memory allocation functionality. In this project, testing can be done using the make test command.
```

## Testing

To execute the test cases, use `make test`. To pull in updated test cases, run `make testupdate`. You can also run a specific test case instead of all of them:

# Run all test cases:
make test

# Run a specific test case:
make test run=4

# Run a few specific test cases (4, 8, and 12 in this case):
make test run='4 8 12'
```
