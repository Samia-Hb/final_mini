
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

Token	**add_token(Token **tokens, TokenType type, char *value)
{
	Token	*new_node;
	Token	*ptr;

	new_node = create_token(type, value);
	// printf("i_before = %d\n", *i);
	// (*i) = (*i) + strlen(value);
	// printf("i_after = %d\n", *i);
	if (!(*tokens))
		*tokens = new_node;
	else
	{
		ptr = *tokens;
		while (ptr->next)
			ptr = ptr->next;
		ptr->next = new_node;
	}
	return (tokens);
}

int ft_is_separtor(char c)
{
	if (c == '>' || c == '<' || c == '|' || c == ' ' || c == '(' || c == ')' || c == '[' || c == ']')
		return (1);
	return (0);
}
char *handle_quote(char *str, char c)
{
    int i = 1;
    int j = 0;
    char *word;

	word = str + 1;
	printf("str = %s\n", str);
	printf("word = %s\n", word);
    if (!strchr(word, c))
	{
        printf("Error: unclosed quote\n");
        exit(1);
    }
	while (str[i] != c && str[i] && !ft_is_separtor(str[i]))
	{
        if (str[i] == '\\' && str[i + 1] == c)
            i += 2;
        i++;
        j++;
    }
    word = malloc(j + 3);
    if (!word)
	{
		printf("Error: memory allocation failed\n");
		exit(1);
    }
    i = 0;
    while (i < j + 2)
	{
        if (str[i] == '\\' && str[i + 1] == c)
            i += 2;
        word[i] = str[i];
		i++;
    }
    word[j + 2] = '\0';
	return word;
}

int get_token_type(char *token)
{
	if(!strcmp(token,"?"))
		return TOKEN_QUESTION;
	else if (!strcmp(token,"~"))
		return TOKEN_TILDLE;
	else if (!strcmp(token,"."))
		return TOKEN_DOT;
	else if (!strcmp(token,">"))
		return TOKEN_REDIR_OUT;
	else if (!strcmp(token,"<"))
		return TOKEN_REDIR_IN;
	else if (!strcmp(token,">>"))
		return TOKEN_REDIR_HERE_DOC;
	else if (!strcmp(token,"|"))
		return TOKEN_PIPE;
	else if (!strcmp(token,"||"))
		return TOKEN_DOUBLE_PIPE;
	else if (!strcmp(token,"&&"))
		return TOKEN_DOUBLE_AMP;
	else if (!strcmp(token,";"))
		return TOKEN_SEMICOLON;
	else if (!strcmp(token,"(") || !strcmp(token,"[") || !strcmp(token,"{"))
		return TOKEN_OPEN_PARENTH;
	else if (!strcmp(token,")") || !strcmp(token,")") || !strcmp(token,")"))
		return TOKEN_CLOSE_PARENTH;
	else if (get_executable(token))
		return TOKEN_COMMAND;
	return TOKEN_UNKNOWN;
}

Token **ft_append_identifier(char *input, Token **token_list, int *tmp, int *j)
{
    int i;
	int k ;
    char *str;
    char *value;

	k = 0;
    str = input;
    i = 0;
	// printf("check\n");
	printf ("i _ value = %d\n", *j);
    while (str[i] && !ft_is_separtor(str[i]))
    {
        if (str[i] == '"' || str[i] == '\'')
        {
			*tmp = 0;
			return (token_list);
        }
        i++;
    }
    value = malloc( i + 1);
	if (!value)
	{
		printf("Error\n");
		exit(1);
	}
	while (k < i)
	{
		value[k] = str[k];
		k++;
	}
	// ft_substr(str, 0 , i);
	*j = strlen(value);
    token_list = add_token(token_list, get_token_type(value), value);
    free(value);
	*input += i;
    return token_list;
}

char	*ft_substr(char *s, unsigned int start, size_t len)
{
	char	*substr;
	size_t	slen;
	size_t	i;

	if (!s)
		return (NULL);
	slen = strlen(s);
	if (!slen || start > slen - 1)
		return (strdup(""));
	i = 0;
	while (s[start + i] && i + 1 <= len)
		i++;
	substr = calloc((i + 1), sizeof(char));
	if (!substr)
		return (NULL);
	i = 0;
	while (i < len && s[start + i])
	{
		substr[i] = s[start + i];
		i++;
	}
	return (substr);
}

char *char_to_string(char c, char c2)
{
	char *string;

	if (!c2)
	{
		string = malloc(2 * sizeof(char));
		string[0] = c;
		string[1] = '\0';
	}
	else
	{
		string = malloc(3 * sizeof(char));
		string[0] = c;
		string[1] = c2;
		string[2] = '\0';
	}
	return (string);
}

Token	**tokenize(char *input)
{
	Token	**tokens;
	int		len;
	int		i;
	int		start = 0;
	char	*arg;
	char	*word;

	tokens = (Token **)malloc(sizeof(Token *));
	*tokens = NULL;
	len = strlen(input);
	i = 0;
	while (input[i])
	{
		if (input[i] == ' ' || input[i] == '\t')
			i++;
		else if ((input[i] == '"' || input[i] == 39) && input[i - 1] != '\\')
		{
			word = handle_quote(input + i, input[i]);
			tokens = add_token(tokens, TOKEN_ARGUMENT, word);
			i += strlen(word);
		}
		else if (input[i] == '~' || input[i] == '?' || input[i] == '&' 
			|| input[i] == '.' || input[i] == '`'|| input[i] == '$'|| input[i] == '>'|| input[i] == '<'|| input[i] == '('
			|| input[i] == '{' || input[i] == '['|| input[i] == ')'|| input[i] == ']' || input[i] == '}' || input[i] == '|' || input[i] == ';')
			{
			tokens = add_token(tokens, get_token_type(char_to_string(input[i], 0)), char_to_string(input[i], 0));
			i++;
			}
		else if ((input[i] == '&' && i + 1 < len && input[i + 1] == '&') || (input[i] == '|' && i + 1 < len && input[i + 1] == '|'))
		{
			tokens = add_token(tokens, get_token_type(char_to_string(input[i] , input[i + 1])), char_to_string(input[i] , input[i + 1]));
			i += 2;
		}
		else if (input[i] == '-')
		{
			printf ("check\n");
			start = i;
			while (i < len && input[i] != ' ' && input[i] != '|'
				&& input[i] != '(' && input[i] != ')')
				i++;
			arg = strndup(input + start, i - start);
			tokens = add_token(tokens, TOKEN_OPTION, arg);
			i = i + (i - start) - 1;
			free(arg);
		}
		else
		{
			start = i;
			while (input[i]  && input[i] != ' ' && input[i] != '|'
				&& input[i] != '(' && input[i] != ')' && input[i] != '&')
				i++;
			word = strndup(input + start , i - start);
			if (get_executable(word))
				tokens = add_token(tokens, TOKEN_COMMAND, word);
			else if (!strcmp(word, ">>"))
				tokens = add_token(tokens, TOKEN_REDIR_APPEND,
						word);
			else if (!strcmp(word, "<<"))
				tokens = add_token(tokens, TOKEN_REDIR_HERE_DOC,
						word);
			else
				tokens = add_token(tokens, TOKEN_ARGUMENT, word);
			i = i + (i - start) - 1;
			free(word);
		}
	}
	return (tokens);
}
// printf("word = %s\n", word);
// printf("strlen(word) = %lu\n", strlen(word));
// printf("input[i] = '%c' \n", input[i]);
// printf(" i_here = %d\n", i);