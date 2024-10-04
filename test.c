#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <fcntl.h>
#include "mysh.h"
#include "lib.h"
#include "messages.h"


void process_pipeline(Job *job);
int main(){
  Job test = {
    { { { "/usr/bin/ls", "-al", "/library", NULL }, 3, 0 },
      { { "/usr/bin/wc", "-l",              NULL }, 2, 0 }
     },
     2,
     "dirlistlen",
     NULL,
     1
  };


  process_pipeline(&test);
}


void process_pipeline(Job *job){
  pid_t pids[10];
  int pipes_needed = job->num_stages - 1;
  int pipefds[2 * pipes_needed];
  int i;

  //create the needed amount of pipes
  for (i = 0; i < job->num_stages; i++)//this 6 is a arbitrary value, we need to make it variable for the amount of commands we are allowing
    {
      if (pipe(pipefds + i * 2) == -1){
      }
    }


  //for every command, we need to fork
  
    // Fork processes for each command
    for (i = 0; i < job->num_stages; i++) {
        pids[i] = fork();
        if (pids[i] == -1) {
        }

        if (pids[i] == 0) {  // Child process
            // If not the first command, redirect input from the previous pipe
            if (i > 0) {
                dup2(pipefds[(i - 1) * 2], STDIN_FILENO);
            }

            // If not the last command, redirect output to the next pipe
            if (i < job->num_stages - 1) {
                dup2(pipefds[i * 2 + 1], STDOUT_FILENO);
            }

            // Close all pipe file descriptors in the child process
            for (int j = 0; j < 2 * (job->num_stages - 1); j++) {
                close(pipefds[j]);
            }

            // Execute the command
            execve(job->pipeline[i].argv[0], job->pipeline[i].argv,NULL);
        }
    }

    // Parent process: Close all pipe file descriptors
    for (i = 0; i < 2 * job->num_stages - 1; i++) {
        close(pipefds[i]);
    }

    // Parent waits for all child processes
    for (i = 0; i < job->num_stages; i++) {
        waitpid(pids[i], NULL, 0);
    }

    return;
}

  
/*
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
85  Function Name: run_command
86  Purpose: runs a given command from the user
87  Details:
         Input: command - the variable containing the command that is to be run
         return: pid - returns the pid of the current process
  */

/*pid_t run_command(Command *command){
    int status;
    pid_t pid;
    pid = fork();

    if (pid == IS_CHILD_PROC)
      execve(command->argv[0], command->argv, NULL);

    else{     waitpid(pid, &status, 0);
   }

  return pid;
};;
*/
