#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>


/*
Things to do:
- Make our own string comparison function
- Move the messages into its own file (.h) named smth like messages.h
- Stop using stdio.h and string.h

Known Bugs:

*/


void clear_buffer(char *buffer, int size){
  for (int i = 0; i < size; i++){
    buffer[i] = '\0';
  }
}

int main(){
    char string_buffer[256];
    char error1 [] = "Exiting Program\n";
    char command_prompt [] = "$ ";
    int bytes_read;

    clear_buffer(string_buffer, 256);
    write(1, command_prompt, 2);
    
    bytes_read = read(0,string_buffer, 256);
    string_buffer[bytes_read-1] = '\0'; /*the read funciton does not null terminate*/
 
    
    while (strcmp(string_buffer, "exit") != 0){
      if(strcmp(string_buffer, "exit") == 0)
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
