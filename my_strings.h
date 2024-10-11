#ifndef MY_STRINGS_H
#define MY_STRINGS_H

#define MAX_TOKENS 256
#include "lib.h"
#include "mysh.h"
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>



int tokenize(char *user_input, char *tokens[MAX_TOKENS]);
int my_strcmp(const char *str1, const char *str2);
void my_strcpy(char *dest, char *src);
int my_strlen(const char *str);



#endif
