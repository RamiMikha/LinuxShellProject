#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>


/*
Things to do:
- Need to make a buffer clear function (set all values inside the buffer to null, using the bytes_read variable as a determining way of how much is needed
- Make our own string comparison function
- Move the messages into its own file (.h) named smth like messages.h

Known Bugs:
- Doesn't print out the prompt upon first boot for some reason
- Since the buffer isn't being cleared after every read, if you type out for example hello and then type t on the next prompt, output is tello
*/

int main(){
    char string_buffer[256];
    char error1 [] = "Exiting Program\n";
    char command_prompt [] = "$ ";
    int bytes_read;
    
    bytes_read = read(0,string_buffer, 256);
    string_buffer[bytes_read-1] = '\0'; /*the read funciton does not null terminate*/
    write(1, command_prompt, 2);
    
    while (strcmp(string_buffer, "exit") != 0){
      if(strcmp(string_buffer, "exit") == 0)
	write(1, error1, 17); /*Exits program*/

      else{
	write(1, string_buffer, 256);
	write(1, "\n", 1);

	write(1, command_prompt, 2);
	bytes_read = read(0,string_buffer,256);
	string_buffer[bytes_read-1] = '\0';
      }
    }
    return 0;
}
