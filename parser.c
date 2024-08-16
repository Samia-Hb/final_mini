# include "minishell.h"

void error_handling(int exit_status)
{
	if (exit_status == '1')
		printf("zsh: parse error near `|'");
	else if (exit_status == 130)
		printf("zsh: Error");
	exit (exit_status);
}

Token *get_last_token(Token *token)
{
	while (token->next != NULL)
	{
		token = token->next;
	}
	return (token);
}

void ft_parser(Token *tokens)
{
	int i;
	Token *last_node;

	i = 0;
	last_node = get_last_token(tokens);
	printf("check\n");
	// while (tokens)
	// {
		if (!strcmp(tokens->value , "|") && i == 0)
			error_handling(1); 
		if (!strcmp(last_node->value , "|"))
			error_handling(130);
		// if ()
		
	// }	
}