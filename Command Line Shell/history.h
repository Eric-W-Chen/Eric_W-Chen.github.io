/**
 * @file
 *
 * Contains shell history data structures and retrieval functions.
 */

#ifndef _HISTORY_H_
#define _HISTORY_H_
/**
 * Functions to interact with history data structures and retrieval functions.
*/
void hist_init(unsigned int);
void hist_destroy(void);
void hist_add(const char *);
void hist_print(void);
const char *hist_search_prefix(char *);
const char *hist_search_cnum(int);
const char *hist_double_bang(int);
unsigned int hist_last_cnum(void);
char * get_history(int);
int get_current_command(void);

#endif