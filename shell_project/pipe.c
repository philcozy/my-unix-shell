#include <signal.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <sys/wait.h>
#include <stdlib.h>
#include "shell.h"

int has_pipe(char* raw_input)
{
  return strchr(raw_input, '|') != NULL;
}
void handle_pipe(char* raw_input)
{
  char* input = space_trim(raw_input);
  char* raw_left = strtok(input, "|");
  char* raw_right = strtok(NULL, "|");

  if(raw_right == NULL)
  {
    fprintf(stderr, "Syntax error: missing command after pipe\n");
    return;
  }

  char* left = space_trim(raw_left);
  char* right = space_trim(raw_right);

  char** left_commands = get_input(left);
  char** right_commands = get_input(right);
  int stat_1;
  int stat_2;
  
  int mypipe[2];
  pipe(mypipe);

  pid_t child_1 = fork(); // Child 1: writer (e.g., ls)
  if(child_1 == 0)
  {
    dup2(mypipe[1], STDOUT_FILENO); // stdout → pipe write-end
    close(mypipe[0]);
    close(mypipe[1]);

    execvp(left_commands[0], left_commands);
  }
  else if(child_1 < 0)
  {
    perror("Fork Failed");
    exit(1);
  }

  pid_t child_2 = fork(); // Child 2: reader (e.g., grep)
  if(child_2 == 0)
  {
    dup2(mypipe[0], STDIN_FILENO); // stdin → pipe read-end
    close(mypipe[1]);
    close(mypipe[0]);

    execvp(right_commands[0], right_commands);
  }
  else if(child_2 < 0)
  {
    perror("Fork Failed");
    exit(1);
  }

  close(mypipe[0]);
  close(mypipe[1]);
  waitpid(child_1, &stat_1, WUNTRACED);
  waitpid(child_2, &stat_2, WUNTRACED);
}



