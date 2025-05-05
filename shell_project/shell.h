#ifndef SHELL_H
#define SHELL_H

#include <setjmp.h>
#include <signal.h>

extern volatile sig_atomic_t jump;
extern sigjmp_buf env;

char** get_input(char* input);
int cd(char* path);
void sigint_handler(int signo);
void father_sigint_settings();
void child_sigint_settings();
void run_command(char **command);
#endif
