
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
#include <errno.h>
#include <stdlib.h>

/*
  background processes seems to be working but for some reason the command prompt does not get written after a background job
  
  
*/

char * path;
char * find_path(char *envp[]);
void build_path(char *path, char *command);
void adjust_job(Job *job);
/*
  Function Name: main
  Purpose: governs the running of the shell (mysh)
  Details: the function calls helper functions and makes system calls that process
  and act based on the provided command line from the user
*/
int main(int argc, char *argv[], char *envp[]){

  Job job;
  //Command command;
  int continue_flag = TRUE;
  pid_t pid;
  int exit_status;
  int i, j = 0;
  //char * path;


  //dont fully get but i think its working to clean up zombie children
  struct sigaction sa;
  sa.sa_handler = &handle_zmbchld; //Set the signal handler function
  sigemptyset(&sa.sa_mask); //stops other handle_zmbchld calls from happening during it is being handles
  sa.sa_flags = SA_RESTART; // tells the OS to restart systems calls in case they get interrupted by nalde_zmbchld
  //this actually installs the function so the OS knows to call it when a child is terminated
  if(sigaction(SIGCHLD, &sa, NULL) == -1){
    write(1,strerror(errno),my_strlen(strerror(errno)));
    write(1,"\n",2);
    _exit(exit_status);
  }



  path = find_path(envp);
  
  //access???  

  
  get_job(&job);

  while (continue_flag){
    if ((job.num_stages == 1) && (job.pipelines[0].numTokens == 1) && (my_strcmp(job.pipelines[0].argv[0], "exit") == 0)){ // this check doesn't work
      continue_flag = FALSE;
    }

    else{
      run_job(&job);
      get_job(&job);
    }
  }  
  write(1, exit_prompt, EXIT_LEN);
  return 0;
}




char * find_path(char *envp[]){
  int i, j, path_index = 0;
  char temp[6];

  for (i = 0; envp[i] != NULL; i++){
    for (j = 0; j < 5; j++){
      temp[j] = envp[i][j];
      printf("%s\n", envp[i]);
    }

    temp[5] = '\0';
    if(my_strcmp(temp, "PATH=") == 0){
      path_index = i;
      break;
    }
  }
  return envp[path_index] + 5;
}


void build_path(char * path, char * command){

  int numTokens = 0;
  char *token_start = NULL;
  char *tokens[numTokens];
  int command_length = my_strlen(command);
  int k = 0;
  
  
  
  
  for (int i = 0; path[i] != '\0'; i++){
    if (path[i] != ':'){
      if (token_start == NULL){
	token_start = &path[i]; //if not white space and token_start is not currently poiting to a token start of a new token
      }
    }else {
      //If we find white space and token_start is pointing to a token, end the token
      if(token_start != NULL){
	int token_length = &path[i] - token_start;
	tokens[numTokens] = (char *)my_malloc(token_length + command_length+ 1);
	if(tokens[numTokens] == NULL) {
	  return; //memory allocation failed
	}
	for (int j = 0; j < token_length; j++) {
	  tokens[numTokens][j] = token_start[j]; //copy the token into the allocated memory
	}
	tokens[numTokens][token_length] = '/'; //append the end of commadn terminate the token
	for (int j = 0; j < token_length+command_length; j++){
	  tokens[numTokens][j+token_length] = command[k];
	}

	tokens[numTokens][token_length + command_length] = '\0'; //Null terminate the token
	k = 0;
	numTokens++;
	token_start = NULL;
      }
    }
  }

  for (int i = 0; i < numTokens; i++){
    if (access(tokens[i], X_OK) == 0){
      my_strcpy(command, tokens[i]);
      i = numTokens;//break out of for loop
    }
  }
  return;
}


void adjust_job(Job *job){
  int i;

  for (i = 0; i < job->num_stages; i++){
    if (strstr(job->pipelines[i].argv[0], "/") == NULL){
      build_path(path, job->pipelines[i].argv[0]);
    }
  }

  return;
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


  for (i = 0; i < MAX_PIPE_LEN; i++){
    for(j = 0; j <  MAX_TOKENS+1; j++){
      job->pipelines[i].argv[j] = NULL;
    }
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
    }

    else{
      if (my_strcmp(job->original_cmd->argv[i], "|") == 0){
	job->num_stages+=1;
      }
      else{
	if(my_strcmp(job->original_cmd->argv[i], "<") == 0){
	  job->infile_path = job->original_cmd->argv[i+1];
	  i++;
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


  //
  //RAMI, if you want to disable command prompt search, do it right here and comment out adjust_job
  //
  adjust_job(job);
  
  return;
};


/*
  Function Name: run_job
  Purpose: to run job initialized by get_job()
  Details: job - the variable containing the job structure to be run
*/
void run_job(Job *job){

  pid_t pid;
  int status;
  int exit_status;
  if(job->num_stages == 1) {

    pid = fork(); //Fork a child proces
    if (pid == -1){
      write(1,strerror(errno),my_strlen(strerror(errno)));
      write(1,"\n",2);
      _exit(exit_status);
    }
    else if (pid == IS_CHILD_PROC) {
      //In child process
      
      //Handling input and output redirection
      handle_input_redirection(job->infile_path);
      handle_output_redirection(job->outfile_path);
      
      if (execve(job->pipelines[0].argv[0], job->pipelines[0].argv, NULL)== -1){
	write(1,strerror(errno),my_strlen(strerror(errno)));
	write(1,"\n",2);
	_exit(exit_status);
      }
    }
    else{
      //in parent process
      if (job->background) {
	waitpid(pid, &status, WNOHANG);
      }
      else {
	//wait for child process to exit if foreground job
	waitpid(pid, &status, 0);
      }    
    }
  }
  else{
    handle_pipes(job);
  }
  return;
};

/*
  Function Name: handle_input_redirection
  Purpose: to handle the input redirection at the start of the job
  Details: infile_path - path to the input file
*/
void handle_input_redirection(const char *infile_path){
  int exit_status;
  if (infile_path != NULL){
    int fd = open(infile_path, O_RDONLY);

    if (fd == -1){
      write(1,strerror(errno),my_strlen(strerror(errno)));
      write(1,"\n",2);
      _exit(exit_status);
    }
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
  int exit_status;
  if(outfile_path != NULL){
    int fd = open(outfile_path, O_WRONLY | O_CREAT | O_TRUNC);

    if (fd == -1){
      write(1,strerror(errno),my_strlen(strerror(errno)));
      write(1,"\n",2);
      _exit(exit_status);
    }
    
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
void handle_pipes(Job *job){
  int status;
  int pipefd[2];
  int prev_fd = STDIN; //start with the standard input
  pid_t pid;
  int exit_status;
  for (int i = 0; i < job->num_stages; i++) {
    if (i < job->num_stages - 1 ){
      //create a pipe if its not the last command
      if (pipe(pipefd)== -1){
	write(1,strerror(errno),my_strlen(strerror(errno)));
	write(1,"\n",2);
	_exit(exit_status);
      }
    }

    pid = fork();

    if (pid == -1){
      write(1,strerror(errno),my_strlen(strerror(errno)));
      write(1,"\n",2);
      _exit(exit_status);
    }
    else if(pid == IS_CHILD_PROC) {
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

      if (execve(job->pipelines[i].argv[0], job->pipelines[i].argv, NULL) == 1){
	write(1,strerror(errno),my_strlen(strerror(errno)));
	write(1,"\n",2);
	_exit(exit_status);
      }
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
      }
      else {
	//wait for child process to exit if foreground job
	waitpid(pid, &status, 0);
      }      
    }
  }
  return;
}
