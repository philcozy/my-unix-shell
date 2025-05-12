#include "shell.h"
#include <stdio.h>
#include <sys/types.h>
#include <readline/readline.h>
#include <stdlib.h>
#include <string.h>

#define CLOSE "\001\033[0m\002"                 // 关闭所有属性
#define BLOD  "\001\033[1m\002"                 // 强调、加粗、高亮
#define BEGIN(x,y) "\001\033["#x";"#y"m\002"    // x: 背景，y: 前景

volatile sig_atomic_t jump = 0;
sigjmp_buf env;

int main()
{
  char** command;
  char* raw_input;

  father_sigint_settings();

  while(1)
  {
    if(sigsetjmp(env, 1) == 42)
    {
      puts("\n");
      continue;
    }

    jump = 1;

    raw_input = readline(BEGIN(49, 34)"unixsh> "CLOSE);

    if(raw_input == NULL) //Ctrl^D
    {
      puts("\n");
      exit(0);
    }

    if(has_pipe(raw_input))
    {
      handle_pipe(raw_input);
      continue;
    }
    else
    {
      command = get_input(raw_input);
    }

    if(strcmp(command[0], "cd") == 0) // strcmp() return 0 when strings are the same
    {
      if(cd(command[1]) < 0) // chdir() return -1 when failed
      {
        perror(command[1]);
      }

      continue; // Skip the fork
    }

    if (strcmp(command[0], "exit") == 0) 
    { // if command is exit
      free(input); // Dont forget free!
      free(command);
      exit(0);
    }

    run_command(command);

    free(raw_input); free(command);
  }
}

