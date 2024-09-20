# include "minishell.h"

int main(int argc, char **argv)
{
	(void)argc;
	char *delimiter = argv[1]; 
 	char *buffer = strdup("");
    while (strcmp(buffer,delimiter))
    {
        write(0, "> ", 2);
        read(STDIN_FILENO,buffer, strlen(delimiter));
    }
}
