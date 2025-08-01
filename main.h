#ifndef MAIN_H
#define MAIN_H

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/wait.h>
extern char **environ;

char *read_line(void);
void execute_cmd(char *cmd);
void display_prompt(void);


#endif /*MAIN_H*/