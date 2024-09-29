/*
Project Members: Brendan Wong and Rami Mikha
*/

#ifndef MYSH_H
#define MYSH_H


#define MAX_TOKENS 256
#define MAX_STRING_LEN 100
#define MAX_READ_LEN 256
#define IS_CHILD_PROC 0


typedef struct{
  char *argv[MAX_TOKENS + 1];
  int numTokens;
  int bytes_read;
}Command;


int tokenize(char *user_input, char *tokens[MAX_TOKENS]);
void get_command(Command *command);
pid_t run_command(Command *command);


#endif


