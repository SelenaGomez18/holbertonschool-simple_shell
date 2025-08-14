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

			if (strcmp(args[0], "echo") == 0 && args[1] && strcmp(args[1], "$?") == 0)
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
			/* Manejo del comando "cd" */
			if (strcmp(args[0], "cd") == 0) /* Verifica si el primer argumento es "cd" */
			{
				char old_pwd[PATH_MAX];  /* Guarda el directorio actual antes de cambiar */
				char new_pwd[PATH_MAX];  /* Guardará el nuevo directorio después de cambiar */

				/* Obtiene el directorio actual y lo guarda en old_pwd */
				if (getcwd(old_pwd, sizeof(old_pwd)) == NULL)
				{
					perror("getcwd");
					free_args(args); /* Libera la memoria de los argumentos */
					free(line);      /* Libera la línea leída */
					last_status = 1; /* Marca error */
					continue;        /* Salta al siguiente ciclo */
				}

				/* Si el usuario escribió solo "cd", cambiar al HOME */
				if (args[1] == NULL)
				{
					char *home = getenv("HOME"); /* Obtiene la variable HOME */
					if (home == NULL)
						home = "/"; /* Si no existe HOME, ir al raíz */

					if (chdir(home) != 0) /* Cambia al directorio HOME */
					{
						perror("cd");
						free_args(args);
						free(line);
						last_status = 1;
						continue;
					}
				}
				else
				{
					/* Cambia al directorio especificado por el usuario */
					if (chdir(args[1]) != 0)
					{
						perror("cd");
						free_args(args);
						free(line);
						last_status = 1;
						continue;
					}
				}

				/* Obtiene el nuevo directorio después del cambio */
				if (getcwd(new_pwd, sizeof(new_pwd)) == NULL)
				{
					perror("getcwd");
					free_args(args);
					free(line);
					last_status = 1;
					continue;
				}

				/* Actualiza las variables de entorno PWD y OLDPWD */
				setenv("OLDPWD", old_pwd, 1);
				setenv("PWD", new_pwd, 1);

				free_args(args); /* Libera memoria antes de continuar */
				free(line);
				last_status = 0; /* Éxito */
				continue; /* Salta a la siguiente iteración del bucle principal */
			}

			count++;
			ret = execute_command(args[0], args, environ, argv[0], count, &last_status);
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
