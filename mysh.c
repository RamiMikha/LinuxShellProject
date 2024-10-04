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
  TO DO:
  - Input Redireciton
  - Pipeline support (for now, just up to 2)
  - I know how to do this since the oode is similar to the lab, however, I want to figure out how to do it n amounts of time

  Some Notes Upon my own thinking - brendan
  - I'm not quite sure how to utilize the data strucutre JOBS to the fullest
  - at the moment, I'm thinking that we might want to like, tokenize EVERYTHING, including the other command signs like |, < , > , & 
  Since it's dynamically allocated already for us, all we have to do is to simply make the Command pipeline[] of the job structure
  to be the splitting up of stuff if that makes sense. For example, we have usr/bin/ls -al | usr/bin/wc -1. In the first part,
  we call tokenize so pipeline[0] will contain the entire command line tokenized. In a loop in main or something, we could check each token 
  until we encounter one of the special ones. Once we do, we set it to 'null' , and we increment the index we use for pipeline[] to 1, where
  we can make use of your strcpy on the rest of the line UP until it runs into another special symbol over and over again.
  

  I'm a bit cooked upon writing this, so for the life of me I have no idea how to do this but I know this works. Possible issues you might have
  with this that I think aren't a worry:
  - Memory leak COULD be an issue BUT i dont think it is. From how you explained to me how our mem allocate works, it's allocating from a very
  specific space (top vs bot). As such, even if we lose the pointer to it, it should be fine since at the end, we are freeing memory anyways
  (AKA, resetting the program break). I believe this makes it so we can unironically get away with losing memory temporarily as we end up
  claiming it all back at the end anyways.

  - I have no idea how to achieve what I'm talking about although I know it should be relatively simple but I cant for the life of me figure out
  how at the time of writing this. In Simple Terms, I want parts of pipeline[0] to be null based on what the string is, however, not turned
  into null until it's copied into pipeline[1] or pipeline[n] in general. This way, we have for example pipeline[0] containing the string for
  usr/bin/ls -al, and pipeline[1] containing usr/bin/wc -1.


  - What I haven't figured out
  - N amounts of pipeline support
  - THe main issue with this in my head is how I want to implement the above. If we do the above, how do we keep track of what input/output redirection there is?
  The job structure itself only keeps track of one set of these between 2 different commands

*/











/*
  Function Name: main
  Purpose: governs the running of the shell (mysh)
  Details: the function calls helper functions and makes system calls that process
  and act based on the provided command line from the user
*/
int main(){

  Job job;
  Command command;
  int continue_flag = TRUE;
  pid_t pid;


  //get_job(&job);
  get_command(&command);
  while(continue_flag){
    if ((command.numTokens == 1) && (my_strcmp(command.argv[0], "exit") == 0)){
      continue_flag = FALSE;
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
  Function Name: get_job
  Purpose: to initialize the job structure
  Details: job - the variable containing the job structure that is to be filled

*/
void get_job(Job *job){
  int i = 0;
  int j;
  //reset contents of get_job
  free_all();
  job->num_stages = 0;
  job->outfile_path = NULL;
  job->infile_path = NULL;
  job->background = 0;
  
  get_command(job->original_cmd);
  job->num_stages++;
  
  for (i = 0; i < job->original_cmd->numTokens; i++){

    if (my_strcmp(job->original_cmd->argv[i], "|") != 0 || my_strcmp(job->original_cmd->argv[i], "<") != 0 ||
	my_strcmp(job->original_cmd->argv[i], ">") != 0 || my_strcmp(job->original_cmd->argv[i], "&") != 0){
      job->pipelines[job->num_stages-1].argv[job->pipelines[job->num_stages-1].numTokens] = job->original_cmd->argv[i]; //if an error occurs, might be here as the numTokens might not be initialized
    }

    else{
      if (my_strcmp(job->original_cmd->argv[i], "|") == 0){
      	  job->num_stages++;
      }

      else{
	if(my_strcmp(job->original_cmd->argv[i], "<") == 0){
	  job->infile_path = job->original_cmd->argv[i+1];
	  i++; //should be +2, but this will run and then the for loop will increment again
	}

	else if(my_strcmp(job->original_cmd->argv[i], ">") == 0){
	  job->outfile_path = job->original_cmd->argv[i+1];
	  i++;
	}

	else if (my_strcmp(job->original_cmd->argv[i], "&") == 0){
	  job->background = TRUE;
	}
      }
    }
  }

  return;
};


/*
  Function Name: run_job
*/
void run_job(Job *job){

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

