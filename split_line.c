#include "main.h"

char **split_line(char *line)
{
    char **tokens = NULL;
    char *token = NULL;
    int i = 0;

    tokens = malloc(sizeof(char *) * 64);
    if (!tokens)
        return (NULL);
    
    token = strtok(line, " \t\r\n");

    while (token != NULL && i < 63)
    {
        tokens[i++] = token;
        token = strtok(NULL, " \t\r\n");
    }
    tokens[i] = NULL;

    return (tokens);
}
