#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "mysh.h"
#include "lib.h"
#include "messages.h"
//#include <stdio.h>
//#include <string.h>

// Still need to make our own strlen function


int numTokens = 0;
int input_index = 0;
int buffer_index = 0;
char *tokens[MAX_TOKENS];


/*
Function Name: tokenize
Purpose: To split up a string into different segments that were separated by whitespace
Details:
 -- RAMI need to do this or explaint ome

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




/*
Function Name: main
Purpose: governs the running of the shell (mysh)
Details: the function calls helper functions and makes system calls that process
         and act based on the provided command line from the user
*/
int main(){
    char string_buffer[MAX_READ_LEN];
    int bytes_read;
    char *tokens[MAX_TOKENS+1];


    numTokens = 0;
    clear_buffer(string_buffer, MAX_READ_LEN);
    write(1, command_prompt, PROMPT_LEN);
    
    bytes_read = read(0,string_buffer, MAX_READ_LEN);
    string_buffer[bytes_read-1] = '\0'; /*the read funciton does not null terminate*/
 
    while (my_strcmp(string_buffer, "exit") != 0){
      if(my_strcmp(string_buffer, "exit") == 0)
	write(1, exit_prompt, EXIT_LEN); /*Exits program*/

      else{
	numTokens = tokenize(string_buffer, tokens); //Tokenize the input

	for (int i = 0; i < numTokens; i++){
	  write (1, tokens[i], my_strlen(tokens[i]));
	  write (1, " ", SPACE_LEN);
	}
	write(1, "\n", 1);

	// Free memory for tokens
	for (int i = 0; i < numTokens; i++){
	  sbrk(-my_strlen(tokens[i]) - 1); 
	}
	
	clear_buffer(string_buffer, MAX_READ_LEN); /*Clear the buffer*/
	write(1, command_prompt, PROMPT_LEN);
	bytes_read = read(0, string_buffer, MAX_READ_LEN);
	string_buffer[bytes_read-1] = '\0';
      }
    }
    write(1, exit_prompt, EXIT_LEN);
    return 0;
}
