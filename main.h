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

/* Core shell functions */
char *read_line(void);
int execute_command(void);
void display_prompt(void);
char *trim_spaces(void);
char **split_line(void);
char *find_full_path(void);
void free_args(void);
int builtin_env(void);
int process_line(void);

/* Built-in command handlers */
int handle_variable_assignment(void);
int handle_echo_status(void);
int handle_exit_command(void);
int handle_env_command(void);
int handle_cd_command(void);

/* CD helper functions */
int cd_to_home(void);
int cd_to_path(void);

#endif /* MAIN_H */
