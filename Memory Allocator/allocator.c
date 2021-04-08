/**
 * @file allocator.c
 *
 * Explores memory management at the C runtime level.
 *
 * Author: Eric Chen and Bae Sung Kim
 *
 * To use (one specific command):
 * LD_PRELOAD=$(pwd)/allocator.so command
 * ('command' will run with your allocator)
 *
 * To use (all following commands):
 * export LD_PRELOAD=$(pwd)/allocator.so
 * (Everything after this point will use your custom allocator -- be careful!)
 */

#include <pthread.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <sys/mman.h>
#include <unistd.h>
#include <limits.h>
#include <stdlib.h>

#include "allocator.h"
#include "logger.h"

// LOG frequently, if you want to turn it off, set LOGGER off in Makefile

static struct mem_block *g_head = NULL; /*!< Start (head) of our linked list */

static unsigned long g_allocations = 0; /*!< Allocation counter */

pthread_mutex_t alloc_mutex = PTHREAD_MUTEX_INITIALIZER; /*< Mutex for protecting the linked list */

/**
 * Allocates the first available memory with space more than or equal to it's size.
 *
 * @param size: size of requested block
 * @return: void pointer to memory location
 * */
void *first_fit(size_t size)
{
	struct mem_block *curr = g_head;
	while (curr != NULL) {	
		int available_bytes = curr->size - curr->usage;

		if (available_bytes > 0 && available_bytes >= size) {
			return curr;
		}
		curr = curr->next;
	}
	return NULL;
}

/**
 * Locates largest available free portion so that the portion left will be big enough to be useful for servicing a future request.
 *
 * @param size: size of requested block 
 * @return: void pointer to memory location
 * */
void *worst_fit(size_t size)
{
	struct mem_block *largest = NULL;
	struct mem_block *curr = g_head;
	int max = 0;

	while (curr != NULL) {
		int available_bytes = curr->size - curr->usage;

		if (size <= available_bytes && available_bytes > max) {
			max = available_bytes;
			largest = curr;
		}
		curr = curr->next;
	}
	return (void *) largest;
}

/**
 * Allocates the smallest possible space in the memory that can accomodate the size requirement of the process
 *
 * @param size: size of requested memory block 
 * @return: void pointer to memory location
 * */
void *best_fit(size_t size)
{
	struct mem_block *block = NULL;
	struct mem_block *curr = g_head;
	int max = INT_MAX;

	while (curr != NULL) {
		int available_bytes = curr->size - curr->usage;

		if (size <= available_bytes && available_bytes < max) {
			max = available_bytes;
			block = curr;
		}
		curr = curr->next;
	}
	return (void *) block;
}

/**
 * Using free space management (FSM) algorithms, find a block of memory that we can reuse. Return NULL if no suitable block is found.
 *
 * @param size: size of requested block that we are inputting into the FSM algorithms
 * return: void pointer to memory location
 * */
void *reuse(size_t size)
{
	char *allocator = getenv("ALLOCATOR_ALGORITHM");
	if (allocator == NULL) {
		allocator = "first_fit";
	}

	struct mem_block *curr = NULL;

	if (strcmp(allocator, "first_fit") == 0) {
		curr = first_fit(size);
	}
	else if (strcmp(allocator, "best_fit") == 0) {
		curr = best_fit(size);
	}
	else if (strcmp(allocator, "worst_fit") == 0) {
		curr = worst_fit(size);
	}
	else {
		return NULL;
	}

	if (curr == NULL) {
		return NULL;
	}

	if (curr->usage == 0) {
		curr -> alloc_id = g_allocations++;
		curr -> usage = size;
		return curr + 1;
	}

	//splitting
	int block_size = curr -> size;
	curr -> size = curr -> usage;

	struct mem_block *carved_block = (void*) curr + curr->usage;

	carved_block -> region_start = curr -> region_start;
	carved_block -> alloc_id = g_allocations++;
	carved_block -> size = block_size - curr -> usage;
	carved_block -> usage = size;
	carved_block -> next = curr -> next;
	curr -> next = carved_block;

	return carved_block + 1;
}

/** 
 * Allocate memory by first checking if we can reuse an existing block. If not, map a new memory region. Also enables Scribble which fills allocated memory with 0xAA which makes it obvious if you're using a memory block after it's free'd by overwriting any data that used to be in the memory block upon free. Keep track of allocations that have been made (increment g_allocations) any time you call malloc. 
 *
 * @param size: size of requested memory block
 * return: void pointer to memory location
 * */
void *malloc(size_t size)
{
	pthread_mutex_lock(&alloc_mutex);
	int scrib = 0;
	size_t usage = size + sizeof(struct mem_block);

	if (getenv("ALLOCATOR_SCRIBBLE") != NULL) {
		scrib = atoi(getenv("ALLOCATOR_SCRIBBLE"));
	}

	if (usage % 8 != 0) {
		usage = usage + (8 - usage % 8);
	}	

	void *ptr = reuse(usage);

	if (ptr != NULL) {
		if (scrib == 1) {
			memset(ptr, 0xAA, size);
		}
		pthread_mutex_unlock(&alloc_mutex);
		return ptr;
	}

	int page_size = getpagesize();
	size_t pages = usage / page_size;

	if (usage % page_size != 0) {
		pages++;
	}

	size_t region_size = pages * page_size;

	struct mem_block *block = mmap ( NULL, 
			region_size, 
			PROT_READ | PROT_WRITE, 
			MAP_PRIVATE | MAP_ANONYMOUS, 
			-1, 
			0) ;

	if (block == MAP_FAILED) {
		pthread_mutex_unlock(&alloc_mutex);
		return NULL;
	}

	block->alloc_id = g_allocations++;
	block->size = region_size;
	block->usage = usage;
	block->region_start = block;
	block->region_size = region_size;
	block->next = NULL;

	if (g_head == NULL) {
		g_head = block;
	}
	else {
		struct mem_block *tail = g_head;
		while (tail -> next != NULL) {
			tail = tail -> next;
		}
		tail->next = block;
	}

	if (scrib == 1) {
		memset(block+1, 0xAA, size);
	}

	pthread_mutex_unlock(&alloc_mutex);
	return block + 1;  //pointing to first byte of actual memory block
}

/** Free memory. If the containing region is empty (i.e., there are no more blocks in use), then it will be unmapped. If an entire region has been freed, call munmap and fix the links between regions
 *
 * @param *ptr: pointer used to access blocks of memory
 * @return: void pointer to memory location
 * */
void free(void *ptr)
{
	pthread_mutex_lock(&alloc_mutex);

	if (ptr == NULL) {
		pthread_mutex_unlock(&alloc_mutex);
		return;
	}

	struct mem_block *prev_p = g_head;
	struct mem_block *block = ((struct mem_block *) ptr - 1);
	block -> usage = 0;
	struct mem_block *region_ptr = block->region_start;
	struct mem_block *region_ptr2 = block->region_start;

	while (region_ptr2 != NULL && region_ptr2->region_start == region_ptr) {
		if (region_ptr2->usage != 0) {
			pthread_mutex_unlock(&alloc_mutex);
			return;
		}
		region_ptr2 = region_ptr2->next;
	}
	
	if (prev_p == region_ptr) {
		g_head = region_ptr2;
	}
	else {
		while (prev_p->next != region_ptr) {
			prev_p = prev_p->next;
		}
		prev_p->next = region_ptr2;
	}

	if (munmap(region_ptr, region_ptr->region_size) == -1) {
		pthread_mutex_unlock(&alloc_mutex);
	}
	
	pthread_mutex_unlock(&alloc_mutex);
}

/**
 * Allocates a specified amount of memory and initializes it to zero. 
 *
 * @param nmemb: array of elements of size bytes each that we're allocating memory for 
 * @param size: size of requested memory block
 * @return: void pointer to memory location
 * */
void *calloc(size_t nmemb, size_t size)
{
	void *ptr = malloc(nmemb * size);
	memset(ptr, 0, nmemb * size);
	return ptr;
}

/**
 * Resizes memory block pointed to by ptr that was previously allocated with a call to malloc or   * calloc.
 *
 * @param *ptr: points to memory block to be resized
 * @param size: size of requested memory block
 * @return: void pointer to newly allocated memory
 * */
void *realloc(void *ptr, size_t size)
{

	if (ptr == NULL) {
		//If the pointer is NULL, then we simply malloc a new block
		return malloc(size);
	}

	if (size == 0) {
		//Realloc to 0 is often the same as freeing the memory block... But the
		//C standard doesn't require this. We will free the block and return
		//NULL here.
		free(ptr);
		return NULL;
	}

	size = size + sizeof(struct mem_block);

	if (size % 8 != 0) {
		size = size + (8 - size % 8);
	}

	struct mem_block *block = ptr - sizeof(struct mem_block);

	if (block->usage < size) {
		if (block->size >= size) {
			block->usage = size;
		}
		else {
			void *new_ptr = malloc(size);
			memcpy(new_ptr, ptr, (block->usage - sizeof(struct mem_block)));
			free(ptr);
			return new_ptr;
		}
	}
	else if (block->usage > size) {
		block->usage = size;
	}
	return (void *) ptr;
}

/** 
 * Function to print the memory blocks that have been free'd
 *
 * @param void
 * @return: void
 * */
void print_free_list(void)
{
	struct mem_block *block_p;

	for (block_p = g_head; block_p != NULL; block_p = block_p->next) {
		LOG("size: %ld\tregion size: %ld\tusage: %ld\n", block_p->size, block_p->region_size, 
				block_p->usage);
	}
}

/**
 * Prints out the current memory state, including both the regions and blocks. Entries are printed in order, * so there is an implied link from the topmost entry to the next, and so on.
 *
 * @param void
 * @return void
 * */
void print_memory(void)
{
	fprintf(stdout, "-- Current Memory State --");
	struct mem_block *current_block = g_head;
	struct mem_block *current_region = NULL;
	while (current_block != NULL) {
		if (current_block->region_start != current_region) {
			current_region = current_block->region_start;
			fprintf(stdout, "[REGION] %p-%p %zu\n",
					current_region,
					(void *) current_region + current_region->region_size,
					current_region->region_size);
		}
		fprintf(stdout, "[BLOCK]  %p-%p (%lu) '%s' %zu %zu %zu\n",
				current_block,
				(void *) current_block + current_block->size,
				current_block->alloc_id,
				current_block->name,
				current_block->size,
				current_block->usage,
				current_block->usage == 0
				? 0 : current_block->usage - sizeof(struct mem_block));
		current_block = current_block->next;
	}
}

/** 
 * Copies name of memory blocks.
 *
 * @param size: size of requested memory blocks 
 * @param name: string array that we're copying to
 * @return: void pointer to memory location
 * */
void * malloc_name(size_t size, const char* name)
{
	void *ptr = malloc(size);
	struct mem_block *block = (struct mem_block *) ptr - 1;
	sprintf(block->name, "%s", name);
	return (void *) ptr;
}
