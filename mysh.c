
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
#include "my_strings.h"
#include <stdio.h>
#include <string.h>



/*
  background processes seems to be working but for some reason the command prompt does not get written after a background job
  
  
*/





/*
  Function Name: main
  Purpose: governs the running of the shell (mysh)
  Details: the function calls helper functions and makes system calls that process
  and act based on the provided command line from the user
*/
int main(){

  Job job;
  //Command command;
  int continue_flag = TRUE;
  pid_t pid;

  //dont fully get but i think its working to clean up zombie children
  struct sigaction sa;
  sa.sa_handler = &handle_zmbchld; //Set the signal handler function
  sigemptyset(&sa.sa_mask); //stops other handle_zmbchld calls from happening during it is being handles
  sa.sa_flags = SA_RESTART; // tells the OS to restart systems calls in case they get interrupted by nalde_zmbchld
  sigaction(SIGCHLD, &sa, NULL);//this actually installs the function so the OS knows to call it when a child is terminated


  get_job(&job);

  while (continue_flag){
    if ((job.num_stages == 1) && (job.pipelines[0].numTokens == 1) && (my_strcmp(job.pipelines[0].argv[0], "exit") == 0)){ // this check doesn't work
      continue_flag = FALSE;
    }

    else{
      pid = run_job(&job);
      if (pid == IS_CHILD_PROC){
	return 0;
      }
      get_job(&job);
    }
  }
  //  get_command(&command);

  /*while(continue_flag){
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
  */  
  write(1, exit_prompt, EXIT_LEN);
  return 0;
}


void handle_zmbchld(int sig){
  int status;

  waitpid(-1,&status, WNOHANG);
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
  int j = 0;
  //reset contents of get_job
  free_all();
  job->num_stages = 0;
  job->outfile_path = NULL;
  job->infile_path = NULL;
  job->background = 0;

  for (i = 0; i < MAX_PIPE_LEN; i++){
    job->pipelines[i].numTokens = 0;
  }
  
  get_command(job->original_cmd);
  job->num_stages++;

  

  
  for (i = 0; i < job->original_cmd->numTokens; i++){

    if (my_strcmp(job->original_cmd->argv[i], "|") != 0 && my_strcmp(job->original_cmd->argv[i], "<") != 0 &&
	my_strcmp(job->original_cmd->argv[i], ">") != 0 && my_strcmp(job->original_cmd->argv[i], "&") != 0){

      if (job->pipelines[job->num_stages-1].argv == NULL)
	job->pipelines[job->num_stages-1].numTokens = 0;

 
      job->pipelines[job->num_stages-1].argv[job->pipelines[job->num_stages-1].numTokens] = job->original_cmd->argv[i]; //if an error occurs, might be here as the numTokens might not be initialized
      job->pipelines[job->num_stages-1].numTokens++;

      printf("What was inputted into pipelines: %s", job->original_cmd->argv[i]);
      printf("\n");
      printf("The value in piplines: %s", job->pipelines[job->num_stages-1].argv[job->pipelines[job->num_stages-1].numTokens]);
      printf("\n");
      printf("numStages is: %i", job->num_stages);
      printf("\n");
    }

    else{
      if (my_strcmp(job->original_cmd->argv[i], "|") == 0){
	job->num_stages+=1;
	printf("numstages incremented: new value is: %i", job->num_stages);
	printf("\n");
      }
      else{
	if(my_strcmp(job->original_cmd->argv[i], "<") == 0){
	  job->infile_path = job->original_cmd->argv[i+1];
	  printf("infile created: value is: %s",job->original_cmd->argv[i]);
	  printf("\n");
	}

	else if(my_strcmp(job->original_cmd->argv[i], ">") == 0){
	  job->outfile_path = job->original_cmd->argv[i+1];
	  printf("outfile_path created: value is: %s", job->original_cmd->argv[i]);
	  printf("\n");
	}

	else if (my_strcmp(job->original_cmd->argv[i], "&") == 0){
	  job->background = TRUE;
	  printf("it is a background process\n");
	}
      }
    }
  }
  
  printf("EXITED GET_JOB\n\n\n\n");
  return;
};


/*
  Function Name: run_job
  Purpose: to run job initialized by get_job()
  Details: job - the variable containing the job structure to be run
*/
pid_t run_job(Job *job){

  pid_t pid;
  printf("entered run_job\n");
  
  
  if(job->num_stages == 1) {

    int status;
    pid = fork(); //Fork a child proces
    if (pid == IS_CHILD_PROC) {

      //In child process
      printf("is a child process\n");
      
      //Handling input and output redirection
      printf("input_redirect function entered\n");
      handle_input_redirection(job->infile_path);

      printf("output_redirect function entered\n");
      handle_output_redirection(job->outfile_path);

      printf("execve entered\n");
      printf("the arguments being entered in order: ");
      printf(" %s", job->pipelines[0].argv[0]);
      printf(" %s", job->pipelines[0].argv);
      printf("\n");
      
      execve(job->pipelines[0].argv[0], job->pipelines[0].argv, NULL);
    }
    else{
      printf("is a parent process\n");
      //in parent process
      if (job->background) {
	printf("is a background execution job\n");
	waitpid(pid, &status, WNOHANG);
	//return;
	//handle_background_execution(job->background);
      }
      else {
	//wait for child process to exit if foreground job
	printf("waitpid entered\n");
	waitpid(pid, &status, 0);
      }    
    }
  }
  else{
    printf("handle_pipes function entering\n");
    pid = handle_pipes(job);
  }
  return pid;
};

/*
  Function Name: handle_input_redirection
  Purpose: to handle the input redirection at the start of the job
  Details: infile_path - path to the input file
*/
void handle_input_redirection(const char *infile_path){
  if (infile_path != NULL){
    printf("input redirecting occurring \n");
    int fd = open(infile_path, O_RDONLY);

    //redirect STDIN to input file
    dup2(fd, STDIN);
    close(fd);
  }
}


/*
  Function Name: handle_OUtput_redirection
  Purpose: to handle the input redirection at the end of the job
  Details: infile_path - path to the output file
*/
void handle_output_redirection(const char *outfile_path) {
  if(outfile_path != NULL){

    printf("output redirect occurring \n");
    int fd = open(outfile_path, O_WRONLY | O_CREAT | O_TRUNC);

    
    //redirect STDOUT to output file
    dup2(fd, STDOUT);
    close(fd);
  }
}


/*
  Function Name: handle_pipes
  Purpose: to handle execution if job has multiple commands with pipelines 
  Details: job - the variable containing the job struct that is to be executed
*/
pid_t handle_pipes(Job *job){
  int status;
  int pipefd[2];
  int prev_fd = STDIN; //start with the standard input
  pid_t pid;
  printf("entering pipe creation\n");
  for (int i = 0; i < job->num_stages; i++) {
    if (i < job->num_stages - 1 ){
      //create a pipe if its not the last command
      pipe(pipefd);
    }

    pid = fork();


    
    if(pid == IS_CHILD_PROC) {
      //In child process

      if (i == 0) {
	//Handling input redirection for the first command
	handle_input_redirection(job->infile_path);
      }else{
	dup2(prev_fd, STDIN); //redirect STDIN to the previous stage
	close(prev_fd);
      }
      
      if (i == job->num_stages - 1){
	//handling output redirection for last command
	handle_output_redirection(job->outfile_path);
      }
      else{
	dup2(pipefd[WRITE], STDOUT);
      }

      //close both pipe ends
      close(pipefd[READ]);
      close(pipefd[WRITE]);

      execve(job->pipelines[i].argv[0], job->pipelines[i].argv, NULL);
    }
    else{
      //in parent process
      if (prev_fd != STDIN) {
	close(prev_fd);
      }
      prev_fd = pipefd[READ]; //save the read enf of the pipe for the next command
      close(pipefd[WRITE]); //close the write end in parent process
      if (job->background) {
	waitpid(pid, &status, WNOHANG);
        //handle_background_execution(job->background);
      }
      else {
        //wait for child process to exit if foreground job
        waitpid(pid, &status, 0);
      }      
    }
  }
  return pid;
}

/*
  Function Name: handle_background_execution
  Purpose: to handle the execution of command in the background
  Details: background - a flag to know if job is to be run in the background
*/
void handle_background_execution(int background){
  return;
}





/*
  Function Name: tokenize
  Purpose: To split up a string into different segments that were separated by whitespace
  Details:
  Input: *user_input - a buffer of characters taken from the user input
  *tokens[] - an array of pointers to token arrays
  return: numTokens - the number of tokens in the current from the user input      

*/

/*
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
/*
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
*/
