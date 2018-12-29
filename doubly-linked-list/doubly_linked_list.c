/* doubly_linked_list.c
 *
 * Implement a sorted linked list of strings with operations Insert 
 * in alphabetical order, Print, Member, Delete, Free_list.
 * The list nodes are doubly linked.
 * 
 * Input:    Single character lower case letters to indicate operations, 
 *           possibly followed by value needed by operation -- e.g. 'i'
 *           followed by "hello" to insert the string "hello" -- no 
 *           double or single quotes.
 *
 * Output:   Results of operations.
 *
 * Compile:  gcc -g -Wall -o doubly_linked_list doubly_linked_list.c
 *           
 * Run:      ./doubly_linked_list
 *
 */

/* You may not add any new header file */

//Eric Chen
//20394954

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Max 99 chars + '\0' */
#define STRING_MAX 100

struct list_node_s {
   char*  data;
   struct list_node_s* prev_p;
   struct list_node_s* next_p;
};

/* Pointers to the head and tail of the list */
struct list_s {
   struct list_node_s* h_p;
   struct list_node_s* t_p;
};

void Insert(struct list_s* list_p, char string[]);
void Print(struct list_s* list_p);
int  Member(struct list_s* list_p, char string[]);
void Delete(struct list_s* list_p, char string[]);
void Free_list(struct list_s* list_p);
char Get_command(void);
void Get_string(char string[]);
void Free_node(struct list_node_s* node_p);
struct list_node_s* Allocate_node(int size);
void Print_node(char title[], struct list_node_s* node_p);


/*-----------------------------------------------------------------*/
int main(void) {
   char          command;
   char          string[STRING_MAX];
   struct list_s list ;  

   list.h_p = list.t_p = NULL;
      /* start with empty list */

   command = Get_command();
   while (command != 'q' && command != 'Q') {
      switch (command) {
         case 'i': 
         case 'I': 
            Get_string(string);
            Insert(&list, string);
            break;
         case 'p':
         case 'P':
            Print(&list);
            break;
         case 'm': 
         case 'M':
            Get_string(string);
            if (Member(&list, string))
               printf("%s is in the list\n", string);
            else
               printf("%s is not in the list\n", string);
            break;
         case 'd':
         case 'D':
            Get_string(string);
            Delete(&list, string);
            break;
         case 'f':
         case 'F':
            Free_list(&list);
            break;
         default:
            printf("There is no %c command\n", command);
            printf("Please try again\n");
      }
      command = Get_command();
   }
   Free_list(&list);
   return 0;
}  /* main */


/*-----------------------------------------------------------------*/
/* Function:   Allocate_node
 * Purpose:    Allocate storage for a list node
 * Input arg:  size = number of chars needed in data member (including
 *                storage for the terminating null)
 * Return val: Pointer to the new node
 */
struct list_node_s* Allocate_node(int size) {

   struct list_node_s* newNode;

   newNode = malloc(sizeof(struct list_node_s)); //allocate memory for list node
   newNode->data = malloc(sizeof(size)); //allocate memory for size of word

   return newNode;
}  /* Allocate_node */


/*-----------------------------------------------------------------*/
/* Function:   Insert
 * Purpose:    Insert new node in correct alphabetical location in list
 * Input arg:  string = new string to be added to list
 * In/out arg: list_p = pointer to struct storing head and tail ptrs
 * Note:       If the string is already in the list, print a message
 *                and return, leaving list unchanged
 */
void Insert(struct list_s* list_p, char string[]) {
   int length;
   int compare;
   struct list_node_s* curr_p;
   struct list_node_s* prev_p;
   struct list_node_s* newNode;

   length = strlen(string); //find length of string

   if (length == 0)
   return;

   //find insertion point
   curr_p = list_p->h_p; //beginning of list
   while (curr_p != NULL) { //go through list
      compare = strcmp(curr_p->data, string);
      if (compare == 0) //making sure there's no duplicates, testing to see if the word is already there
      {
         printf("%s is already in the list\n", string);
         return; 
      }
      else if (compare > 0) //found insertion point
         break;
      curr_p = curr_p->next_p;
   }

   newNode = Allocate_node(length + 1); //allocate memory for list node and for length of word
   strcpy(newNode->data, string); //copy word into node

   if (curr_p == NULL)
   {  //insert at the end of the list
   newNode->prev_p = list_p->t_p; //set the prev_p pointer to the last item in the list
   list_p->t_p = newNode; //at the end of the list
   //we are now linked in properly in the backwards direction
   newNode->next_p = NULL; //last one on the list is null
   prev_p = newNode->prev_p; //this is the old end of the list
   if (prev_p == NULL) //if we have a predecessor
      list_p->h_p = newNode; // make list head point to us
   else
      prev_p->next_p = newNode; // make predecessor point to us
   //we are now linked in properly in the forwards direction
   }

   else
   {  //insert the word before curr_p
      newNode->next_p = curr_p;//pointing forward
      newNode->prev_p = curr_p->prev_p; //pointing backwards
      curr_p->prev_p = newNode; //have the node point back to us
      prev_p = newNode->prev_p; //this is the old previous node
      if (prev_p == NULL) //if we have a predecessor
         list_p->h_p = newNode; // make list head point to us
      else
         prev_p->next_p = newNode; // make predecessor point to us
      //we are now linked in properly in the forwards direction
   }
}  /* Insert */

/*-----------------------------------------------------------------*/
/* Function:   Print
 * Purpose:    Print the contents of the nodes in the list
 * Input arg:  list_p = pointers to first and last nodes in list
 *
 * Hint: The implementation of this function (below) shows how
 * to traverse a linked list.
 */
void Print(struct list_s* list_p) {
   struct list_node_s* curr_p = list_p->h_p;

   printf("list = ");

   while (curr_p != NULL) {
      printf("%s ", curr_p->data);
      curr_p = curr_p->next_p;
   }
   printf("\n");
}  /* Print */


/*-----------------------------------------------------------------*/
/* Function:   Member
 * Purpose:    Search list for string
 * Input args: string = string to search for
 *             list_p = pointers to first and last nodes in list
 * Return val: 1, if string is in the list, 0 otherwise
 */
int  Member(struct list_s* list_p, char string[]) {
   struct list_node_s* curr_p;
   int compare;

   curr_p = list_p->h_p; //beginning of list
   while (curr_p != NULL) { //iterating through list
      compare = strcmp(curr_p->data, string);
      if (compare == 0) //if the word is already there
         return 1; //string is in list
      else if (compare > 0) //gone past where the word should be
         break;
      curr_p = curr_p->next_p;
   }
   return 0; //string not in list

}  /* Member */


/*-----------------------------------------------------------------*/
/* Function:   Free_node
 * Purpose:    Free storage used by a node of the list
 * In/out arg: node_p = pointer to node to be freed
 */
void Free_node(struct list_node_s* node_p) {
   if(node_p == NULL)
      return; //nothing to free if node is null

   if(node_p->data != NULL) 
      free(node_p->data); //freeing the word space

   free(node_p); //freeing the node
}  /* Free_node */


/*-----------------------------------------------------------------*/
/* Function:   Delete
 * Purpose:    Delete node containing string.
 * Input arg:  string = string to be deleted
 * In/out arg  list_p = pointers to head and tail of list
 * Note:       If the string is in the list, it will be unique.  So
 *             at most one node will be deleted.  If the string isn't
 *             in the list, the function just prints a message and
 *             returns, leaving the list unchanged.
 */
void Delete(struct list_s* list_p, char string[]) {
   int length;
   int compare;
   struct list_node_s* curr_p;
   struct list_node_s* prev_p;
   struct list_node_s* next_p;

   length = strlen(string); //find length of string

   if (length == 0)
   return;

   //find the word
   curr_p = list_p->h_p; //beginning of list
   while (curr_p != NULL) { //iterate through list
      compare = strcmp(curr_p->data, string);
      if (compare == 0) //if the word is already there
         break;
      else if (compare > 0) //gone past where the word should be
      {
         printf("%s is not in the list\n", string);
         return;
      }

      curr_p = curr_p->next_p;
   }
   
   if (curr_p == NULL)
   {
      printf("%s is not in the list\n", string);
      return; //got to the end of the list without finding the word
   }

   //curr_p points to the word to be deleted
   next_p = curr_p->next_p; //this is the next node after the word
   prev_p = curr_p->prev_p; //this is the previous node before the word

   if (next_p == NULL) //if we're at the end of the list
      list_p->t_p = prev_p; //sets tail to the previous node
   else
      next_p->prev_p = prev_p; 
   //have removed ourselves from the backwards direction

   if (prev_p == NULL) //if we have a predecessor
      list_p->h_p = next_p; // make list head point to us
   else
      prev_p->next_p = next_p; // make predecessor point to us
   //have removed ourselves from the forward direction

   Free_node(curr_p); //free memory for length of word and for list node
     /* Delete */
}
/*-----------------------------------------------------------------*/
/* Function:   Free_list
 * Purpose:    Free storage used by list
 * In/out arg: list_p = pointers to head and tail of list
 */
void Free_list(struct list_s* list_p) {

   struct list_node_s* curr_p;
   struct list_node_s* next_p;

   curr_p = list_p->h_p; //start off head pointer
   next_p = list_p->h_p; //start off head pointer

   while (curr_p != NULL) {
      next_p = curr_p->next_p; //make a note of the next node
      Free_node(curr_p); //get rid of the current node
      curr_p = next_p; //set current node to the next node
   }

   //makes the system properly initialized
   list_p->h_p = NULL; //head is null
   list_p->t_p = NULL; //tail is null


}  /* Free_list */


/*-----------------------------------------------------------------*/
/* Function:   Get_command
 * Purpose:    Find and return the next non-whitespace character in
 *             the input stream
 * Return val: The next non-whitespace character in stdin
 */
char Get_command(void) {
   char c;

   printf("Please enter a command (i, d, m, p, f, q):  ");
   /* Put the space before the %c so scanf will skip white space */
   scanf(" %c", &c);
   return c;
}  /* Get_command */

/*-----------------------------------------------------------------*/
/* Function:   Get_string
 * Purpose:    Read the next string in stdin (delimited by whitespace)
 * Out arg:    string = next string in stdin
 */
void Get_string(char string[]) {

   printf("Please enter a string:  ");
   scanf("%s", string);
}  /* Get_string */


/*-----------------------------------------------------------------*/
/* Function:  Print_node
 * Purpose:   Print the data member in a node or NULL if the
 *            pointer is NULL
 * In args:   title:  name of the node
 *            node_p:  pointer to node
 */
void Print_node(char title[], struct list_node_s* node_p) {
   printf("%s = ", title);
   if (node_p != NULL)
      printf("%s\n", node_p->data);
   else
      printf("NULL\n");
}  /* Print_node */

