#include "main.h"

int main(int argc, char **argv)
{
	char **args;
	char *line = NULL, *clean_line;
	int count = 0;
	int last_status = 0;
	int ret;

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
		if (clean_line[0] == '#' || (clean_line[0] == '/' && clean_line[1] == '#'))
		{
    		free(line);
    		continue;
		}
		if (clean_line[0] != '\0')
		{
			args = split_line(clean_line);
			if (!args)
			{
				free(line);
				continue;
			}

			if (strcmp(args[0], "echo") == 0 && args[1] && strcmp(args[1], "$?") == 0)
			{
				char buffer[12];
				sprintf(buffer, "%d\n", last_status);
				write(STDOUT_FILENO, buffer, strlen(buffer));
				free_args(args);
				free(line);
				continue;
			}
			
			if (strcmp(args[0], "exit") == 0)
			{
				free_args(args);
				free(line);
				exit(last_status);
			}

			count++;
			ret = execute_command(args[0], args, environ, argv[0], count, &last_status);
			free_args(args);

			if (ret == -1)
			{
				if (!isatty(STDIN_FILENO))
					exit(last_status);
			}
		}

		free(line);
	}

	if (isatty(STDIN_FILENO))
		write(STDOUT_FILENO, "OK\n", 3);

	return (0);
}
