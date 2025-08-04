#include "main.h"

char *find_full_path(char *command)
{
    char *path_env = NULL, *path_copy = NULL, *token = NULL, *full_path = NULL;
    int len;

    if (access(command, X_OK) == 0)
        return (strdup(command));
    
    path_env = getenv("PATH");
    if (!path_env)
        return(NULL);
    
    path_copy = strdup(path_env);
    if (!path_copy)
        return (NULL);
    
    token = strtok(path_copy, ":");
    while (token)
    {
        len = strlen(token) + strlen(command) + 2;
        full_path = malloc(len);
        if (!full_path)
        {
            free(path_copy);
            return (NULL);
        }
        sprintf(full_path, "%s%s", token, command);

        if (access(full_path, X_OK) == 0)
        {
            free(path_copy);
            return (full_path);
        }
        free(full_path);
        token = strtok(NULL, ":");
    }
    free(path_copy);
    return (NULL);
}
