#include "minishell.h"

char *get_command_path(char *command)
{
    char *path = get_executable(command);
}

void expand(Token *tokens)
{
    Token *temp;

    temp = tokens;
    while (temp)
    {
        if (temp->type == TOKEN_COMMAND)
            temp->expanded_value = get_executable(temp->value);
        else if(temp->type == TOKEN_TILDLE)
    }
}