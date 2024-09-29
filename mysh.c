/*
Project Members: Brendan Wong and Rami Mikha
*/

#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <fcntl.h>
#include "mysh.h"
#include "lib.h"
#include "messages.h"

//#include <stdio.h>
//#include <string.h>

/*
Function Name: main
Purpose: governs the running of the shell (mysh)
Details: the function calls helper functions and makes system calls that process
         and act based on the provided command line from the user
*/
int main(){
    Command command;
    int continue_flag = 1;
    pid_t pid;

    get_command(&command);
    while(continue_flag){
      if ((command.numTokens == 1) && (my_strcmp(command.argv[0], "exit") == 0)){
	continue_flag = 0;
      }

      else{
	pid = run_command(&command);
	//exits if child process did not change properly to new process
	if (pid == IS_CHILD_PROC){
	  write(1, invalid_command, INVALID_LEN);
	  return 0;
	}
	get_command(&command);
      }
    }    
    write(1, exit_prompt, EXIT_LEN);
    return 0;
}




/*
Function Name: get_command
Purpose: retrieves a command from the user
Details: 
       Input: command - the variable containing the command structure that is to be populated    
*/
void get_command(Command *command){
  char string_buffer[MAX_READ_LEN];
  free_all(); //clears contents of argv
  
  write(1, command_prompt, PROMPT_LEN);

  command->bytes_read = read(0, string_buffer, MAX_READ_LEN);
  string_buffer[command->bytes_read-1] = '\0';
  command->numTokens = tokenize(string_buffer, command->argv);  

  return;
};



/*
Function Name: run_command
Purpose: runs a given command from the user
Details: 
       Input: command - the variable containing the command that is to be run
       return: pid - returns the pid of the current process 
*/
pid_t run_command(Command *command){
  int status;
  pid_t pid;
  pid = fork();

  if (pid == IS_CHILD_PROC)
    execve(command->argv[0], command->argv, NULL);

  else{
    waitpid(pid, &status, 0);
  }
  
  return pid;
};




/*
Function Name: tokenize
Purpose: To split up a string into different segments that were separated by whitespace
Details:
       Input: *user_input - a buffer of characters taken from the user input
              *tokens[] - an array of pointers to token arrays
       return: numTokens - the number of tokens in the current from the user input      

*/
int tokenize(char *user_input, char *tokens[MAX_TOKENS+1]){
  int numTokens = 0;
  char *token_start = NULL; //pointer to the start of the token

  for (int i = 0; user_input[i] != '\0'; i++){
    if (user_input[i] != ' '){
      if (token_start == NULL){
	token_start = &user_input[i]; //if not white space and token_start is not currently poiting to a token start of a new token
      }
    }else {
      //If we find white space and token_start is pointing to a token, end the token
      if(token_start != NULL){
	int token_length = &user_input[i] - token_start;
	tokens[numTokens] = (char *)my_malloc(token_length + 1);
	if(tokens[numTokens] == NULL) {
	  return -1; //memory allocation failed
	}
	for (int j = 0; j < token_length; j++) {
	  tokens[numTokens][j] = token_start[j]; //copy the token into the allocated memory
	}
	tokens[numTokens][token_length] = '\0'; //Null terminate the token
	numTokens++;
	token_start = NULL;
      }
    }
  }

  /*Handling the case if we reach the end of input without encountering a whitespace*/
  if (token_start != NULL) {
    int token_length = &user_input[my_strlen(user_input)] - token_start;
    tokens[numTokens] = (char *)my_malloc(token_length + 1);
    if (tokens[numTokens] == NULL){
      return -1; //memory allocation failed
    }
    for (int j = 0; j < token_length; j++){
      tokens[numTokens][j] = token_start[j];
    }
    tokens[numTokens][token_length] = '\0';
    numTokens++;
    
  }

  tokens[numTokens] = NULL;
  return numTokens;  
}

