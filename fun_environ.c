#include <unistd.h>

/**
 * builtin_env - Prints the current environment variables
 * Return: 0 on success
 */
int builtin_env(void)
{
    int i = 0;

    while (environ[i])
    {
        write(STDOUT_FILENO, environ[i], strlen(environ[i]));
        write(STDOUT_FILENO, "\n", 1);
        i++;
    }
    return (0);
}
