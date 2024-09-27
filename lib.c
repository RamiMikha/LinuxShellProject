#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "lib.h"


/*same == 0
difference != 0*/

/*
Function Name: my_strcmp
Purpose: compares the contents of two strings
Details:
      Input: str1 - the first string that is to be compared to of type char *
             str2 - the second string that is to be compared to of type char *
      Output: outputs the result of the comparison, where 0 means the strings are the same,
              and any other value means that the strings are different
*/
int my_strcmp(const char *str1, const char *str2){
  while (*str1 && *str2) {
    if (*str1 != *str2) {
      /*If the characters are different return the difference*/
      return *str1 - *str2;

    }

    /*Increment to the next set of characters*/
    str1++;
    str2++;
  }

  /*If we reach this point in means there is no difference in the strings or one string is longer than the other so we compare one final time*/
  return *str1 - *str2;
}



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
Function Name: my_strcpy
Purpose: to make a copy of the original string
Details:
       Input: dest - where the original string is to be copied over to, of type char *
              src - the original string that is to be copied, of type char *
*/
void my_strcpy(char *dest, char *src){
  while (*src){
    *dest++ = *src++;
  }

  *dest = '\0';
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



/*
Function Name: my_strlen
Purpose: to determine the length of a given string
Details:
       Input: str - the string that is to be measured
*/
int my_strlen(const char *str){
  int length = 0;
  while (str[length] != '\0'){
    length++;
  }
  return length;
}

