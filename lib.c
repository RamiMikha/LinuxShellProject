#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "lib.h"


/*same == 0
difference != 0*/
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


void clear_buffer(char *buffer, int size){
  for (int i = 0; i < size; i++){
    buffer[i] = '\0';
  }
}

void my_strcpy(char *dest, char *src){
  while (*src){
    *dest++ = *src++;
  }

  *dest = '\0';
}

void *my_malloc(int size){
  //get current break (end of heap)
  void *current_brk = sbrk(0);

  
  if (sbrk(size) == (void *)-1){ //allocate size number of bytes of memory 
    return NULL; ///retrun NULL if failed
  }

  return current_brk; //return pointer to newly allocated memory
}

int my_strlen(const char *str){
  int length = 0;
  while (str[length] != '\0'){
    length++;
  }
  return length;
}
