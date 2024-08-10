
#include "minishell.h"

void print_token(Token *tokens)
{
    if (!tokens)
		return;
	while (tokens)
    {
		printf("value = %s: type = %d\n", tokens->value ,tokens->type);
    	tokens = tokens->next;
	}
}


Token	*create_token(TokenType type, char *value)
{
	Token	*token;

	token = (Token *)malloc(sizeof(Token));
	token->type = type;
	token->value = strdup(value);
	token->next = NULL;
	return (token);
}

Token	**add_token(Token **tokens, int *token_count, TokenType type,
		char *value)
{
	Token	*new_node;
	Token	*ptr;

	new_node = create_token(type, value);
	(*token_count)++;
	if (!(*tokens))
	{
		*tokens = new_node;
	}
	else
	{
		ptr = *tokens;
		while (ptr->next)
		{
			ptr = ptr->next;
		}
		ptr->next = new_node;
	}
	return (tokens);
}

char *handle_quote(char *str, char c)
{
    int i = 1;
    int j = 0;
    char *word;

	while (str[i] != c && str[i] != '\0')
	{
        if (str[i] == '\\' && str[i + 1] == c)
            i += 2;
        i++;
        j++;
    }
    if (str[i] != c)
	{
        printf("Error: unclosed quote\n");
        exit(1);
    }
    word = malloc(j + 2);
    if (!word)
	{
        printf("Error: memory allocation failed\n");
        exit(1);
    }
    i = 0;
    j = 0;
    while (str[i])
	{
        if (str[i] == '\\' && str[i + 1] == c)
            i += 2;
        else
            word[j++] = str[i++];
    }
    word[j] = '\0';
    return word;
}

Token	**tokenize(char *input, int *token_count)
{
	Token	**tokens;
	int		len;
	int		i;
	int		start = 0;
	char	*arg;
	char	*word;

	tokens = (Token **)malloc(sizeof(Token *));
	*tokens = NULL;
	*token_count = 0;
	len = strlen(input);
	i = 0;
	while (input[i])
	{
		if (input[i] == ' ')
			i++;
		else if (input[i] == '?')
		{
			tokens = add_token(tokens, token_count, TOKEN_QUESTION, "?");
			i++;
		}
		else if ((input[i] == '"' || input[i] == 39) && input[i - 1] != '\\')
		{
			word = handle_quote(input + i, input[i]);
			tokens = add_token(tokens, token_count, TOKEN_ARGUMENT, word);
			i = i + strlen(word);
		}
		else if (input[i] == '~')
		{
			tokens = add_token(tokens, token_count, TOKEN_TILDLE, "~");
			i++;
		}
		else if (input[i] == '.')
		{
			tokens = add_token(tokens, token_count, TOKEN_DOT, ".");
			i++;
		}
		else if (input[i] == '`')
		{
			tokens = add_token(tokens, token_count, TOKEN_BACKTICK, "`");
			i++;
		}
		else if (input[i] == '$')
		{
			tokens = add_token(tokens, token_count, TOKEN_DOLLAR, "$");
			i++;
		}
		else if (input[i] == '>')
		{
			tokens = add_token(tokens, token_count, TOKEN_REDIR_OUT, ">");
			i++;
		}
		else if (input[i] == '<')
		{
			tokens = add_token(tokens, token_count, TOKEN_REDIR_IN, "<");
			i++;
		}
		else if (input[i] == '(')
		{
			tokens = add_token(tokens, token_count, TOKEN_OPEN_PARENTH, "(");
			i++;
		}
		else if (input[i] == ')')
		{
			tokens = add_token(tokens, token_count, TOKEN_CLOSE_PARENTH, ")");
			i++;
		}
		else if (input[i] == '&' && i + 1 < len && input[i + 1] == '&')
		{
			tokens = add_token(tokens, token_count, TOKEN_DOUBLE_AMP, "&&");
			i += 2;
		}
		else if (input[i] == '|' && i + 1 < len && input[i + 1] == '|')
		{
			tokens = add_token(tokens, token_count, TOKEN_DOUBLE_PIPE, "||");
			i += 2;
		}
		else if (input[i] == '|')
		{
			tokens = add_token(tokens, token_count, TOKEN_PIPE, "|");
			i++;
		}
		else if (input[i] == ';')
		{
			tokens = add_token(tokens, token_count, TOKEN_SEMICOLON, ";");
			i++;
		}
		else if (input[i] == '-')
		{
			start = i;
			while (i < len && input[i] != ' ' && input[i] != '|'
				&& input[i] != '(' && input[i] != ')')
				i++;
			arg = strndup(input + start, i - start);
			tokens = add_token(tokens, token_count, TOKEN_OPTION, arg);
			free(arg);
		}
		else
		{
			start = i;
			while (i < len && input[i] != ' ' && input[i] != '|'
				&& input[i] != '(' && input[i] != ')' && input[i] != '&')
				i++;
			word = strndup(input + start, i - start);
			if (get_executable(word))
				tokens = add_token(tokens, token_count, TOKEN_COMMAND, word);
			else if (!strcmp(word, ">>"))
				tokens = add_token(tokens, token_count, TOKEN_REDIR_APPEND,
						word);
			else if (!strcmp(word, "<<"))
				tokens = add_token(tokens, token_count, TOKEN_REDIR_HERE_DOC,
						word);
			else
				tokens = add_token(tokens, token_count, TOKEN_ARGUMENT, word);
			free(word);
		}
	}
	return (tokens);
}