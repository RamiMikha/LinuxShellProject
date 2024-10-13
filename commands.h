/*
File Name: commands.h
Professor: Marc Schroeder
Course: COMP 3659
Project Members: Brendan Wong and Rami Mikha
*/

#ifndef COMMANDS_H
#define COMMANDS_H


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
#include "mysh.h"


#define MAX_PIPE_LEN 10
#define STDOUT 1
#define ERROR_CODE -1

typedef struct{
  char *argv[MAX_TOKENS + 1];
  int numTokens;
  int bytes_read;
}Command;

typedef struct{
  Command pipelines[MAX_PIPE_LEN];
  Command original_cmd[1];
  unsigned int num_stages;
  char *outfile_path;
  char *infile_path;
  int background;
}Job;



void get_command(Command *command);
void get_job(Job *job);
pid_t run_command(Command *command);
void run_job(Job *job, char *envp[]);
void handle_input_redirection(const char *infile_path);
void handle_output_redirection(const char *outfile_path);
void handle_pipes(Job *job, char *envp[]);
void handle_background_execution(int background);
void handle_zmbchld(int sig);





#endif
