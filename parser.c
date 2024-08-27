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
		return (1);
	return (0);
}

int handle_quotes(Token *tokens)
{
	while (tokens)
	{
		if (!check_quote(tokens->value, '"') || !check_quote(tokens->value, '\''))
		{
			printf ("Syntax Error: unclosed quoted\n");
			return (1);
		}
		tokens = tokens->next;
	}
	return (0);
}

int handle_parentheses(Token *token)
{
	Token *current;
    int open_parentheses;
	
	open_parentheses = 0;
	current = token;
	if (current->type == TOKEN_OPEN_PARENTH)
        open_parentheses++;
    else if (current->type == TOKEN_CLOSE_PARENTH)
        open_parentheses--;
    if (open_parentheses < 0)
    {
        printf("Syntax Error: Unmatched closing parenthesis.\n");
        return 1;
    }
	return (0);
}

int handle_consecutive_operator(Token *tokens)
{
	Token *current;
	Token *prev;

	current = tokens->next;
	prev = tokens;
	while (current)
	{
		 if ((prev->type == TOKEN_PIPE || prev->type == TOKEN_DOUBLE_PIPE ||
                 prev->type == TOKEN_DOUBLE_AMP || prev->type == TOKEN_REDIR_OUT ||
                 prev->type == TOKEN_REDIR_IN || prev->type == TOKEN_REDIR_APPEND ||
                 prev->type == TOKEN_REDIR_HERE_DOC) &&
                (current->type == TOKEN_PIPE || current->type == TOKEN_DOUBLE_PIPE ||
                 current->type == TOKEN_DOUBLE_AMP || current->type == TOKEN_REDIR_OUT ||
                 current->type == TOKEN_REDIR_IN || current->type == TOKEN_REDIR_APPEND ||
                 current->type == TOKEN_REDIR_HERE_DOC))
            {
                printf("Syntax Error: Consecutive operators detected.\n");
                return 1;
            }
		prev = tokens;
		tokens = tokens->next;
	}
	return (0);
}

int lst_size(Token *stack)
{
	int size;

	size = 0;
	while (stack)
	{
		size++;
		stack = stack->next;
	}
	return (size);
}

int handle_operators_bg_en(Token *tokens)
{
	Token *last_node;

	last_node = get_last_token(tokens);
	if ((is_operator(tokens) && lst_size(tokens) == 1) || is_operator(last_node) || tokens->type == TOKEN_PIPE || tokens->type == TOKEN_DOUBLE_AMP ||tokens->type == TOKEN_DOUBLE_PIPE)
	{
		printf("Syntax Error.\n");
		return (1);
	}
	return (0);
}

void ft_strncpy(char *dest, char *src, int n)
{
	int i;

	i = 0;
    while (i < n) 
	{
        dest[i] = src[i];
		i++;
	}
	dest[i] = '\0';
    return (dest);
}

int check_token(char *str, char c)
{
	int		i;
	int		j;
	char	*copied;

	i = 1;
	j = 0;
	while (str[i] && str[i] != c)
	{
		i++;
		j++;
	}
	copied = malloc(j + 1);
	ft_strncpy(copied,str,j + 1);
	if (is_operator(copied))
		return (1);
	return (0);
}
char quote_type(char *str)
{
	char c;
	int i;

	i = 0;
	while (str[i] && (str[i] == ' ' || str[i] == '\t'))
		i++;
	if (str[i] == '"' || str[i] == '\'')
		c = str[i];
	return (c);
}
void update_tokens(Token **tokens)
{
	char quote;

	while (*tokens)
	{
		if ((*tokens)->double_qutoed || (*tokens)->single_quoted)
		{
			quote = quote_types(*tokens);
			if (check_token(*tokens,quote))
				ft_strtrim(tokens, quote);
			
		}
		(*tokens) = (*tokens)->next;
	}
}

int check_syntax_errors(Token *tokens)
{
	if (handle_quotes(tokens))
		return(1);
	update_tokens(&tokens);
	if (handle_parentheses(tokens))
		return(2);
	if (handle_parentheses(tokens))
		return (3);
	if (handle_operators_bg_en(tokens))
		return (5);
	if (handle_consecutive_operator(tokens))
		return (4);
	printf ("check\n");
    return (0);
}