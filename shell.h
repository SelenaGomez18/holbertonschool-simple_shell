#ifndef SHELL_H
#define SHELL_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <sys/stat.h>
#include <errno.h>

/* environ global */
extern char **environ;
/* global exit flag set by builtin_exit to request shell termination */
extern int exit_flag;


/* prompt and main helpers */
void prompt(const char *ps1);
int handle_line(char *line, char *prog_name, unsigned int *line_number);

/* parsing */
char **split_line(char *line);
void free_tokens(char **tokens);

/* path lookup and execution */
char *find_command_path(char *cmd);
int execute_cmd(char **args, char *prog_name, unsigned int *line_number);

/* builtins */
int is_builtin(char **args, char *prog_name, unsigned int *line_number);
int builtin_env(char **args);
int builtin_exit(char **args);

/* errors */
void print_error_not_found(char *prog_name, unsigned int line_num, char *cmd);

#endif /* SHELL_H */
