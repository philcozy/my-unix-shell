#include "shell.h"
#include <signal.h>
#include <setjmp.h>
#include <unistd.h>
#include <setjmp.h>

void sigint_handler(int signo)
{
  (void)signo; //Explicity ignore the parameter
  
  if(!jump)
  {
    return;
  }

  siglongjmp(env, 42);
}

void father_sigint_settings()
{
  struct sigaction s;
  s.sa_handler = sigint_handler;
  sigemptyset(&s.sa_mask);
  s.sa_flags = SA_RESTART;
  sigaction(SIGINT, &s, NULL);
}

void child_sigint_settings()
{
  struct sigaction s_child;
  s_child.sa_handler = SIG_DFL;
  sigemptyset(&s_child.sa_mask);
  s_child.sa_flags = SA_RESTART;
  sigaction(SIGINT, &s_child, NULL);
}
