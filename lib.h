/*
Project Members: Brendan Wong and Rami Mikha
*/

#ifndef LIB_H
#define LIB_H

#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "lib.h"


static void *heap_start = NULL; //Pointer to track the initial program break

/*same == 0
difference != 0*/
//int my_strcmp(const char *str1, const char *str2);
void clear_buffer(char *buffer, int size);
//void my_strcpy(char *dest, char *src);
void *my_malloc(int size);
void free_all();
//int my_strlen(const char *str);

#endif


		  
