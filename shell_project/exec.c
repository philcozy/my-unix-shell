#include "shell.h"
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>

void run_command(char **command)
{
  pid_t child_pid;
  int stat;

  child_pid = fork();

  if(child_pid < 0)
  {
    perror("Fork Failed");
    exit(1);
  }

  if(child_pid == 0)
  {
    child_sigint_settings();

    if(execvp(command[0], command) < 0)
    {
      perror(command[0]);
      exit(1);
    }
  }
  else
  {
    waitpid(child_pid, &stat, WUNTRACED);
  }
}
