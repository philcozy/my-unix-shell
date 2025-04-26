#include <stdio.h>
#include <sys/types.h>
#include <readline/readline.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>
#include <setjmp.h>

#define CLOSE "\001\033[0m\002"                 // 关闭所有属性
#define BLOD  "\001\033[1m\002"                 // 强调、加粗、高亮
#define BEGIN(x,y) "\001\033["#x";"#y"m\002"    // x: 背景，y: 前景

static volatile sig_atomic_t jump = 0;
static sigjmp_buf env;

char** get_input(char* input);
int cd(char* path);
void sigint_handler(int signo);

int main()
{
  char** command;
  pid_t child_pid;
  char* input;
  int stat;

    struct sigaction s;
    s.sa_handler = sigint_handler;
    sigemptyset(&s.sa_mask);
    s.sa_flags = SA_RESTART;
    sigaction(SIGINT, &s, NULL);

  while(1)
  {
    if(sigsetjmp(env, 1) == 42)
    {
      puts("\n");
      continue;
    }

    jump = 1;

    input = readline(BEGIN(49, 34)"unixsh> "CLOSE);

    if(input == NULL) //Ctrl^D
    {
      puts("\n");
      exit(0);
    }

    command = get_input(input);

    if(strcmp(command[0], "cd") == 0) // strcmp() return 0 when strings are the same
    {
      if(cd(command[1]) < 0) // chdir() return -1 when failed
      {
        perror(command[1]);
      }

      continue; // Skip the fork
    }

    child_pid = fork();

    if(child_pid < 0)
    {
      perror("Fork Failed");
      exit(1);
    }

    if(child_pid == 0)
    {
      struct sigaction s_child;
      s_child.sa_handler = SIG_DFL;
      sigemptyset(&s_child.sa_mask);
      s_child.sa_flags = SA_RESTART;
      sigaction(SIGINT, &s_child, NULL);

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

  free(input); free(command);
}


char** get_input(char* input)
{
  int buffsize = 8;
  char** commands = malloc(buffsize * sizeof(char*));

  if(commands == NULL)
  {
    perror("Malloc Fails");
    exit(1);
  }

  char* parsed = strtok(input, " ");
  int index = 0;

  while(parsed != NULL)
  {
    commands[index] = parsed;
    index++;

    if(index > buffsize)
    {
      buffsize *= 2;
      commands = realloc(commands, buffsize * sizeof(char*));
      if(commands == NULL)
      {
        perror("Realloc");
        exit(1);
      }
    }

    parsed = strtok(NULL, " ");
  }

  commands[index] = NULL;
  return commands;
}

int cd(char* path)
{
  return chdir(path);
}

void sigint_handler(int signo)
{
  if(!jump)
  {
    return;
  }

  siglongjmp(env, 42);
}
