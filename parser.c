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
	if (token == NULL)
		return NULL;
	while (token->next != NULL)
		token = token->next;
	return (token);
}

int check_quote(char *str, char c)
{
	int count;
	int	i;

	count = 0;
	i = 0;
	while (str[i])
	{
		if (str[i] == c)
			count++;
		i++;
	}
	if (!(count % 2))
		return 1;
	return (0);
}

void ft_parser(Token *tokens)
{
	int		i;
	Token	*last_node;

	i = 0;
	last_node = get_last_token(tokens);
	while (tokens)
	{
		if (!strcmp (tokens->value , "|") && i == 0)
			error_handling(1); 
		if (!strcmp (last_node->value , "|"))
			error_handling(130);
		if (!check_quote(tokens->value, '"') || !check_quote(tokens->value, '\''))
		{
			printf ("unclosed quoted");
			exit(130);
		}
		tokens = tokens->next;	
	}	
}