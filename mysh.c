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
//static char buffer [MAX_TOKENS][MAX_STRING_LEN + 1];


// I MADE THIS FOR A PREVIOUS ATTEMPT I HAD AT DOING THIS
void write_to_screen(){
  char *start = tokens;
  int processed = 0;
  input_index = 0;
  while(processed < numTokens){
    while(tokens[processed][input_index] != '\0'){
      write(1, &tokens[processed][input_index], 1);
      input_index++;
    }
    processed++;
    input_index = 0;
  }
  return;
}



//DOES NOT TOKENIZE, INCOMPLETE
void tokenize(char *user_input){
  //static char buffer [MAX_TOKENS][MAX_STRING_LEN + 1];
  char *start = tokens;
  int continue_running = 1;
  buffer_index = 0;
  input_index = 0;
  numTokens = 0;

  printf("tokenize, before while loop\n");
  while (continue_running){
    if (user_input[input_index] != '\0'){
      *start | user_input[input_index];
      start++;
      buffer_index++;
      if (buffer_index >= MAX_TOKENS)
	continue_running = 0;
    }


  }
  return;
}



// DOES NOT TOKENIZE
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
	write(1, string_buffer, 256);
	write(1, "\n", 1);

	clear_buffer(string_buffer, 256); /*Clear the buffer*/
	write(1, command_prompt, 2);
	bytes_read = read(0,string_buffer,256);
	string_buffer[bytes_read-1] = '\0';
      }
    }
    return 0;
}
