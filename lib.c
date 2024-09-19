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
