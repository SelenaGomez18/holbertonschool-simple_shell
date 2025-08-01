#include "main.h"

int main(void)
{
    char *line = NULL;

    while (1)
    {
        display_prompt();
        line = read_line();

        if (line == NULL)
        {
            if (isatty(STDIN_FILENO))
                write(STDOUT_FILENO, "\n", 1);
            break;
        }

        if (line[0] != '\0')
            execute_cmd(line);

        free(line);
    }
    
    return(0);
}