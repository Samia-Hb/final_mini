#include "minishell.h"

typedef struct command
{
	char	*command;
	char	**arguments;
	char	*operator;
}t_parser;

void analyse_tokens(Token **tokens)
{
    int errno;
    Token *new_tokens;

    errno = check_syntax_errors(*tokens);
    if (errno)
        exit(errno);
    while (tokens)
    {
        if ((*tokens)->type == command)
        {

        }
    }
}