/*
File Name: mysh.h
Professor: Marc Schroeder
Course: COMP 3659
Project Members: Brendan Wong and Rami Mikha
*/

#ifndef MYSH_H
#define MYSH_H


#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include "lib.h"
#include "messages.h"
#include "my_strings.h"
#include "commands.h"


#define MAX_TOKENS 256
#define MAX_STRING_LEN 100
#define MAX_READ_LEN 256
#define IS_CHILD_PROC 0
#define TRUE 1
#define FALSE 0
#define WRITE 1
#define READ 0
#define STDOUT 1
#define STDIN 0
#define EXIT_CODE -1
#define MAX_PATH_LEN 1024
#define MAX_DIR_LEN 256


char * find_path(char *envp[]);
int build_path(char * path, char *command, char * full_path);

#endif


