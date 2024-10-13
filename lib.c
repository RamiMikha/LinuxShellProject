/*
File Name: lib.c
Professor: Marc Schroeder
Course: COMP 3659
Project Members: Brendan Wong and Rami Mikha
*/

#include "lib.h"



/*
Function Name: clear_buffer
Purpose: clears the contents of a buffer (an array)
Details:
       Input: buffer - the array that is to have its contents cleared, of type char *
              size - the fill amount of the buffer being provided, of type int
       Output: clears all the contents of the buffer and makes the contents null
*/
void clear_buffer(char *buffer, int size){
  for (int i = 0; i < size; i++){
    buffer[i] = '\0';
  }
}



/*
Function Name: my_malloc
Purpose: to allocate memory
Details:
       Input: size - the desired amount of bytes of memory that is to be allocated
*/
void *my_malloc(int size){

  if (heap_start == NULL) {
    heap_start = sbrk(0);
  }
  
  //get current break (end of heap)
  void *current_brk = sbrk(0);

  
  if (sbrk(size) == (void *)-1){ //allocate size number of bytes of memory 
    return NULL; ///retrun NULL if failed
  }

  return current_brk; //return pointer to newly allocated memory
}


/*
Function Name: my_free
Purpose: to free up allocated memory
Details:
       Input: original_brk - a pointer that has the address of the original program break
*/


void free_all() {
  if (heap_start != NULL) {
    //reset the program's break back to the initial break
    brk(heap_start);
  }
}
