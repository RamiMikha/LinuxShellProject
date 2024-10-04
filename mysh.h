/*
Project Members: Brendan Wong and Rami Mikha
*/

#ifndef MYSH_H
#define MYSH_H


#define MAX_TOKENS 256
#define MAX_STRING_LEN 100
#define MAX_READ_LEN 256
#define IS_CHILD_PROC 0
#define MAX_PIPE_LEN 10
#define TRUE 1
#define FALSE 0

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


int tokenize(char *user_input, char *tokens[MAX_TOKENS]);
void get_command(Command *command);
void get_job(Job *job);
pid_t run_command(Command *command);
void run_job(Job *job);

#endif


