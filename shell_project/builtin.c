#include "shell.h"
#include <unistd.h>

int cd(char* path)
{
  return chdir(path);
}
