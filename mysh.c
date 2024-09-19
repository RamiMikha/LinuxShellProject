#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "mysh.h"
#include "lib.h"
#include <stdio.h>

/*
  Things to do:
  - Move the messages into its own file (.h) named smth like messages.h
  - Make a strlen function (determine string length)
  - NEED TO MAKE A strtok like funciton (I'm stuck on this - Brendan)
  Known Bugs:

*/


int numTokens = 0;
int last_index = 0;


char *tokenize(char *user_input){
  
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
