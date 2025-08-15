#include "main.h"

int main(int argc, char **argv)
{
	char **args;
	char *line = NULL;
	char *clean_line;
	int count = 0;
	int last_status = 0;
	int ret;
	char *target_dir;        /* Para cd */
    char old_pwd[1024];      /* Guarda directorio actual antes de cambiar */
    char new_pwd[1024];      /* Guarda el nuevo directorio después de cambiar */


	(void)argc;

	while (1)
	{
		if (isatty(STDIN_FILENO))
			display_prompt();

		line = read_line();

		if (line == NULL)
		{
			if (isatty(STDIN_FILENO))
				write(STDOUT_FILENO, "\n", 1);
			break;
		}

		clean_line = trim_spaces(line);

		if (clean_line[0] == '#')
			goto end_iteration;

		if (strncmp(clean_line, "export", 6) == 0 &&
			(clean_line[6] == ' ' || clean_line[6] == '\t' || clean_line[6] == '\0'))
			goto end_iteration;

		if (strchr(clean_line, '=') != NULL &&
			(isalnum((unsigned char)clean_line[0]) || clean_line[0] == '_'))
		{
			char *name = strtok(clean_line, "=");
			char *value = strtok(NULL, "");
			if (name && value)
				setenv(name, value, 1);
			else if (name)
				setenv(name, "", 1);
			goto end_iteration;
		}

		if (clean_line[0] != '\0')
		{
			args = split_line(clean_line);
			if (!args)
				goto end_iteration;

			if (strcmp(args[0], "echo") == 0 && args[1] &&
				strcmp(args[1], "$?") == 0)
			{
				char buffer[12];
				sprintf(buffer, "%d\n", last_status);
				write(STDOUT_FILENO, buffer, strlen(buffer));
				free_args(args);
				goto end_iteration;
			}

			if (strcmp(args[0], "exit") == 0)
			{
				free_args(args);
				free(line);
				exit(last_status);
			}

			if (strcmp(args[0], "env") == 0)
			{
				builtin_env();
				free_args(args);
				free(line);
				continue;
			}

			/* Built-in: cd */
			if (strcmp(args[0], "cd") == 0)
			{
				if (args[1] == NULL)
					target_dir = getenv("HOME");
				else if (strcmp(args[1], "-") == 0)
					target_dir = getenv("OLDPWD");
				else
					target_dir = args[1];

				if (target_dir == NULL)
				{
					fprintf(stderr, "cd: no such file or directory\n");
				}
				else
				{
					if (getcwd(old_pwd, sizeof(old_pwd)) == NULL)
						perror("getcwd");

					if (chdir(target_dir) != 0)
					{
						perror("cd");
					}
					else
					{
						if (old_pwd[0] != '\0')
							setenv("OLDPWD", old_pwd, 1);

						if (getcwd(new_pwd, sizeof(new_pwd)) != NULL)
							setenv("PWD", new_pwd, 1);

						if (args[1] && strcmp(args[1], "-") == 0)
							printf("%s\n", new_pwd);
					}
				}

				free_args(args);
				free(line);
				continue;
			}

			count++;
			ret = execute_command(args[0], args, environ,
				argv[0], count, &last_status);
			free_args(args);

			if (ret == -1 && !isatty(STDIN_FILENO))
			{
				free(line);
				exit(last_status);
			}
		}

		end_iteration:
		free(line);
		line = NULL;
	}

	if (isatty(STDIN_FILENO))
		write(STDOUT_FILENO, "OK\n", 3);

	return (last_status);
}
