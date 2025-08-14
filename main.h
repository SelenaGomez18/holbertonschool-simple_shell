#ifndef MAIN_H
#define MAIN_H

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <string.h>
#include <errno.h>
#include <signal.h>
#include <ctype.h>
#include <limits.h>

extern char **environ;

char *read_line(void);
int execute_command(char *command, char **args, char **env, char *shell_name, int count, int *last_status);
void display_prompt(void);
char *trim_spaces(char *str);
char **split_line(char *line);
char *find_full_path(char *command);
void free_args(char **args);
int builtin_env(void);

#endif /*MAIN_H*/