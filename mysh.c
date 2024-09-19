#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>


/*
Things to do:
- Move the messages into its own file (.h) named smth like messages.h

Known Bugs:

*/


int my_strcmp(const char *str1, const char *str2){
  while (*str1 && *str2) {
    if (*str1 != *str2) {
      /*If the characters are different return the difference*/
      return *str1 - *str2;
      
    }

    /*Increment to the next set of characters*/
    str1++;
    str2++;
  }

  /*If we reach this point in means there is no difference in the strings or one string is longer than the other so we compare one final time*/
  return *str1 - *str2;
}

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
