
/*
  File Name: mysh.c
  Professor: Marc Schroeder
  Course: COMP 3659
  Project Members: Brendan Wong and Rami Mikha
*/

#include "mysh.h"

/*
  Function Name: main
  Purpose: governs the running of the shell (mysh)
  Details: the function calls helper functions and makes system calls that process
  and act based on the provided command line from the user
*/
int main(int argc, char *argv[], char *envp[]){

  Job job;
  int continue_flag = TRUE;
  pid_t pid;
  int exit_status;
  int i, j = 0;
  struct sigaction sa;

  sa.sa_handler = &handle_zmbchld; //Set the signal handler function
  sigemptyset(&sa.sa_mask); //stops other handle_zmbchld calls from happening during it is being handles
  sa.sa_flags = SA_RESTART; // tells the OS to restart systems calls in case they get interrupted by nalde_zmbchld
  //this actually installs the function so the OS knows to call it when a child is terminated
  if(sigaction(SIGCHLD, &sa, NULL) == EXIT_CODE){
    write(STDOUT,strerror(errno),my_strlen(strerror(errno)));
    write(STDOUT,"\n",2);
    _exit(exit_status);
  }
  
  get_job(&job);

  while (continue_flag){
    if ((job.num_stages == 1) && (job.pipelines[0].numTokens == 1) && (my_strcmp(job.pipelines[0].argv[0], "exit") == 0)){
      continue_flag = FALSE;
    }

    else{
      run_job(&job, envp);
      get_job(&job);
    }
  }  
  write(STDOUT, exit_prompt, EXIT_LEN);
  return 0;
}



/*
  Function Name: find_path
  Purpose: to find the PATH
  Details: 
         Input: envp - the variable that contains all the environments
	 Output: returns a pointer to where the path is contained in envp
*/
char * find_path(char *envp[]){
  int i, j, path_index = 0;
  char temp[6];

  for (i = 0; envp[i] != NULL; i++){
    for (j = 0; j < 5; j++){
      temp[j] = envp[i][j];
    }

    temp[5] = '\0';
    if(my_strcmp(temp, "PATH=") == 0){
      path_index = i;
      break;
    }
  }
  return envp[path_index] + 5;
}



/*
  Function Name: build_path
  Purpose: to build the command path for a provided command
  Details:
         Input: path - the variable that points to the PATH
	        command - the command that doesn't contain a path
		full_path - the variable that contains the full path
	 Output: returns 0 if a valid path was not found, and 1 if it was
*/
int build_path(char * path, char * command, char *full_path){

  int path_len = my_strlen(path);
  int cmd_len = my_strlen(command);
  int dir_start = 0;
  int dir_len;
  struct stat buf;
  char dir[MAX_DIR_LEN];

  //iterate through PATH and tokenize with ':'
  for(int i = 0; i <= path_len; i++){
    //check for : or end of string
    if(path[i] == ':' || path[i] == '\0'){
      dir_len = i - dir_start;
      //copy directory from path to dir buffer
      if (dir_len > 0 && dir_len < MAX_DIR_LEN){
	my_strcpy(dir, &path[dir_start]);
	dir[dir_len] = '\0';

	//make the full path
	my_strcpy(full_path, dir);
	full_path[dir_len] = '/';
	my_strcpy(&full_path[dir_len + 1], command);
	full_path[dir_len + 1 + cmd_len] = '\0';

	//using stat system call to check if the command exists and if it can be executed
	if (stat(full_path, &buf) == 0 && (buf.st_mode & S_IXUSR)){
	  return 1; //return if we find the command
	} 
      }
      //if not found, move on to next directory
      dir_start = i + 1;
    }
  }
  return 0; //did not find command
}
