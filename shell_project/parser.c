#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "shell.h"

char** get_input(char* raw_input)
{
  int buffsize = 8;
  char** commands = malloc(buffsize * sizeof(char*));

  if(commands == NULL)
  {
    perror("Malloc Fails");
    exit(1);
  }
  
  char* input = space_trim(raw_input);
  char* parsed = strtok(input, " ");
  int index = 0;

  while(parsed != NULL)
  {
    commands[index] = parsed;
    index++;

    if(index >= buffsize)
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

char* space_trim(char* raw_input) //removes leading and trailing spaces
{
  while(*raw_input == ' ')
  {
    raw_input++;
  }

  if(*raw_input == 0)
  {
    return raw_input;
  }
  
  char* end = raw_input + strlen(raw_input) - 1;

  while(end > raw_input && *end == ' ')
  {
    end--;
  }

  *(end + 1) = 0;

  return raw_input;

}

