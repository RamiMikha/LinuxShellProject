#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "mysh.h"
#include "lib.h"
#include <stdio.h>
#include <string.h>
/*
  Things to do:
  - Move the messages into its own file (.h) named smth like messages.h
  - Make a strlen function (determine string length)
  - NEED TO MAKE A strtok like funciton (I'm stuck on this - Brendan)
     = my thoughts: potentially, we could use bitwise and treat tokens like a buffer (inefficient)
     = maybe we need a buffer that can store it temporarily, and have tokens point to address in that buffer
        i attempted this, but it didn't work. I'm not quite sure why

  Known Bugs:

*/


int numTokens = 0;
int input_index = 0;
int buffer_index = 0;
char *tokens[MAX_TOKENS];


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



int main(){
    char string_buffer[256];
    char error1 [] = "Exiting Program\n";
    char command_prompt [] = "$ ";
    int bytes_read;
    char *tokens[MAX_TOKENS+1];


    numTokens = 0;
    clear_buffer(string_buffer, 256);
    write(1, command_prompt, 2);
    
    bytes_read = read(0,string_buffer, 256);
    string_buffer[bytes_read-1] = '\0'; /*the read funciton does not null terminate*/
 
    while (my_strcmp(string_buffer, "exit") != 0){
      if(my_strcmp(string_buffer, "exit") == 0)
	write(1, error1, 17); /*Exits program*/

      else{
	numTokens = tokenize(string_buffer, tokens); //Tokenize the input

	for (int i = 0; i < numTokens; i++){
	  write (1, tokens[i], strlen(tokens[i]));
	  write (1, " ", 1);
	}
	write(1, "\n", 1);

	// Free memory for tokens
	for (int i = 0; i < numTokens; i++){
	  sbrk(-strlen(tokens[i]) - 1); 
	}
	
	clear_buffer(string_buffer, 256); /*Clear the buffer*/
	write(1, command_prompt, 2);
	bytes_read = read(0,string_buffer,256);
	string_buffer[bytes_read-1] = '\0';
      }
    }
    return 0;
}
