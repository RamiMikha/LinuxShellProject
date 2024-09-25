#ifndef MYSH_H
#define MYSH_H


#define MAX_TOKENS 256
#define MAX_STRING_LEN 100
#define MAX_READ_LEN 256
#define IS_CHILD_PROC 0
int tokenize(char *user_input, char *tokens[MAX_TOKENS]);

#endif


