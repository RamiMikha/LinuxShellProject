#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "mysh.h"
#include "lib.h"


/*
Things to do:
- Move the messages into its own file (.h) named smth like messages.h
- Make a strlen function (determine string length)

Known Bugs:

*/

#define MAX_TOKENS 256


int tokenize(char *tokens[], char *user_input){
  int numTokens = 0;
  int i = 0;
  int temp_index = 0;
  char temp_token[256];

  while (my_strcmp(user_input, "\0") != 0){
    temp_token[temp_index] = user_input[i];
    temp_index++;
    i++;


    /*maybe bitwise operation????*/
    if (my_strcmp(user_input[i]," ") == 0){
      /*add numTokens index as a global variable in the .h*/
      tokens[numTokens] = temp_token;
    }
  }
  
}


int main(){
    char string_buffer[256];
    char error1 [] = "Exiting Program\n";
    char command_prompt [] = "$ ";
    int bytes_read;

    char *tokens[MAX_TOKENS+1];
    
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
