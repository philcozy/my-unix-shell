#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "shell.h"

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
