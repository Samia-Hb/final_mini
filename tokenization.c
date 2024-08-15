
#include "minishell.h"

Token *create_token(TokenType type, const char *value)
{
    Token *token;
	
	token = malloc(sizeof(Token));
    if (!token)
	{
        printf("Error: Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }
    token->type = type;
    token->value = strdup(value);
    token->next = NULL;
    return token;
}

void add_token(Token **tokens, TokenType type, const char *value)
{
	Token	*new_node;
	Token	*ptr;

	new_node = create_token(type, value);
    if (!(*tokens))
        *tokens = new_node;
    else
	{
        ptr = *tokens;
        while (ptr->next)
		{
            ptr = ptr->next;
        }
        ptr->next = new_node;
    }
}

void free_tokens(Token *tokens)
{
    Token *current;
    Token *next;

	current = tokens;
    while (current)
	{
        next = current->next;
        free(current->value);
        free(current);
        current = next;
	}
}

int ft_is_separator(char c)
{
    return strchr("><| ()[]", c) != NULL;
}

char *handle_Parentheses(char *str, char c)
{
    int i = 1;
    int j = 0;
    char *word;
    char close_parenth;

    if (c == '(')
        close_parenth = ')';
    else if (c == '[')
        close_parenth = ']';
    else
        close_parenth = '}';
    while (str[i] && str[i] != close_parenth)
    {
        if (str[i] == '\\' && str[i + 1] == c)
            i += 2;
        else
            i++;
        j++;
    }
    if (str[i] != close_parenth)
    {
        fprintf(stderr, "Error: unclosed quote\n");
        exit(EXIT_FAILURE);
    }
    word = malloc(j + 3);
    if (!word) {
        fprintf(stderr, "Error: memory allocation failed\n");
        exit(EXIT_FAILURE);
    }
    word[0] = c;
    strncpy(word + 1, str + 1, j);
    word[j + 1] = close_parenth;
    word[j + 2] = '\0';
    return word;
}

char *handle_quote(char *str, char c)
{
	int i = 1;
    int j = 0;
    char *word;

    while (str[i] && str[i] != c)
	{
        if (str[i] == '\\' && str[i + 1] == c)
            i += 2;
        else
            i++;
        j++;
    }
    if (str[i] != c)
	{
        fprintf(stderr, "Error: unclosed quote\n");
        exit(EXIT_FAILURE);
    }
    word = malloc(j + 3);
    if (!word)
	{
        fprintf(stderr, "Error: memory allocation failed\n");
        exit(EXIT_FAILURE);
    }
    word[0] = c;
    strncpy(word + 1, str + 1, j);
    word[j + 1] = c;
    word[j + 2] = '\0';
    return word;
}

int get_token_type(const char *token)
{
    if (!strcmp(token, "?"))
        return TOKEN_QUESTION;
    if (!strcmp(token, "~"))
        return TOKEN_TILDLE;
    if (!strcmp(token, "."))
        return TOKEN_DOT;
    if (!strcmp(token, "<<"))
        return TOKEN_REDIR_HERE_DOC;
    if (!strcmp(token, ">"))
        return TOKEN_REDIR_OUT;
    if (!strcmp(token, "<"))
        return TOKEN_REDIR_IN;
    if (!strcmp(token, ">>"))
        return TOKEN_REDIR_APPEND;
    if (!strcmp(token, "|"))
        return TOKEN_PIPE;
    if (!strcmp(token, "||"))
        return TOKEN_DOUBLE_PIPE;
    if (!strcmp(token, "&&"))
        return TOKEN_DOUBLE_AMP;
    if (!strcmp(token, ";"))
        return TOKEN_SEMICOLON;
    if (!strcmp(token, "(") || !strcmp(token, "[") || !strcmp(token, "{"))
        return TOKEN_OPEN_PARENTH;
    if (!strcmp(token, ")") || !strcmp(token, "]") || !strcmp(token, "}"))
        return TOKEN_CLOSE_PARENTH;
    return TOKEN_UNKNOWN;
}

Token **ft_append_identifier(char *input, Token **token_list, int *tmp, int *j) {
    int i = 0;
    int k = 0;
    char *value;

    while (input[i] && !ft_is_separator(input[i]))
	{
        if (input[i] == '"' || input[i] == '\'')
		{
            *tmp = 0;
            return token_list;
        }
        i++;
    }
    value = malloc(i + 1);
    if (!value)
	{
        fprintf(stderr, "Error: memory allocation failed\n");
        exit(EXIT_FAILURE);
    }
    while (k < i)
	{
        value[k] = input[k];
        k++;
    }
    value[k] = '\0';
    *j = i;
    add_token(token_list, get_token_type(value), value);
    free (value);
    return token_list;
}

char *char_to_string(char c, char c2)
{
    char *string;

    if (!c2)
	{
        string = malloc(2 * sizeof(char));
        if (!string)
		{
            printf("Error: memory allocation failed\n");
            exit(EXIT_FAILURE);
        }
        string[0] = c;
        string[1] = '\0';
    }
	else
	{
        string = malloc(3 * sizeof(char));
        if (!string) {
            printf("Error: memory allocation failed\n");
            exit(EXIT_FAILURE);
        }
        string[0] = c;
        string[1] = c2;
        string[2] = '\0';
    }
    return string;
}

int ft_strchr(char *string, char *delimiteur, int *l)
{
    int		i;
    char	**splited;

    splited = ft_split(string, '\n');
    i = 0;
    if (!splited)
    {
        printf("Error\n");
        return (0);
    }
    while (splited[i])
    {
        *l += strlen(splited[i]) ;
        if (!strcmp(splited[i], delimiteur))
            return (1);
        i++;
    }
    return (0);
}

int find_delimiter_in_lines(char *string, char *delimiter, int *l)
{
    int     i;
    char    **splitted;

	i = 0;
	while (string[i] && string [i] == '<' && string[i] == ' ')
		i++;
	l += i;
    splitted = ft_split(string, '\n');
    if (!splitted)
	{
        printf("Error\n");
        return (0);
    }
	i = 0;
    while (splitted[i])
	{
        *l = *l + strlen(splitted[i]);
        if (!strcmp(splitted[i], delimiter))
            return (1);
        i++;
    }
    return (0);
}

char *heredoc_token(char *input, int l)
{
    char	*full_token;
    int		i;

	i = 0;
    full_token = malloc(l * sizeof(char));
    if (!full_token)
        return NULL;
    while (i < l)
	{
        full_token[i] = input[i];
		i++;
	}
	return (full_token);
}

char *handle_heredoc(char *input)
{
    int     i = 0;
    int     j = 0;
    int     k = 0;
    char    *delimiter;
    int     l = 0;

    while (input[i] && (input[i] == '<' || input[i] == ' '))
        i++;
    k = i;
    while (input[i] != ' ' && input[i] != '\0' && input[i] != '\n')
        i++;
    j = i - k;
    delimiter = malloc(j + 1);
    if (!delimiter)
	{
        perror("Error\n");
        exit(1);
    }
	l = i;
    strncpy(delimiter, input + j, k);
    delimiter[j] = '\0';
    if (!find_delimiter_in_lines(input, delimiter, &l))
	{
        printf("Undefined Delimiter in here-doc\n");
        free(delimiter);
        exit(1);
    }
    free(delimiter);
    return heredoc_token(input, l);
}

Token **tokenize(char *input) {
    Token **tokens;
    int len;
    int i = 0;
    int start = 0;
    char *word;

    tokens = (Token **)malloc(sizeof(Token *));
    if (!tokens)
	{
        printf("Error: Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }
    *tokens = NULL;
    len = strlen(input);
    while (input[i])
    {
        if (input[i] == ' ' || input[i] == '\t') {
            i++;
        }
        else if (input[i] == '<' && input[i + 1] && input[i+1] == '<')
        {
            word = handle_heredoc(input+i);
            add_token(tokens, TOKEN_REDIR_HERE_DOC, word);
            i += strlen(word);
        }
        else if ((input[i] == '(' || input[i] == '[' || input[i] == '{') && (input[i - 1] != '\\'))
        {
            word = handle_Parentheses(input + i, input[i]);
            add_token(tokens, TOKEN_ARGUMENT, word);
            i += strlen(word);
            free(word);
        }
        else if ((input[i] == '"' || input[i] == '\'') && (i == 0 || input[i - 1] != '\\'))
        {
            word = handle_quote(input + i, input[i]);
            add_token(tokens, TOKEN_ARGUMENT, word);
            i += strlen(word);
            free(word);
        }else if (strchr("~?&.`$><(){}[]|;", input[i])) {
            char *op = char_to_string(input[i], 0);
            add_token(tokens, get_token_type(op), op);
            free(op);
            i++;
        } else if ((input[i] == '&' && i + 1 < len && input[i + 1] == '&') ||
                   (input[i] == '|' && i + 1 < len && input[i + 1] == '|')) {
            char *op = char_to_string(input[i], input[i + 1]);
            add_token(tokens, get_token_type(op), op);
            free(op);
            i += 2;
        }
        else if (input[i] == '-') {
            start = i;
            while (i < len && input[i] != ' ' && input[i] != '|' && !ft_is_separator(input[i])) {
                i++;
            }
            word = strndup(input + start, i - start);
            add_token(tokens, TOKEN_OPTION, word);
            free(word);
        } else {
            start = i;
            while (input[i] && !ft_is_separator(input[i]) && input[i] != ' ' && input[i] != '&')
                i++;
            word = strndup(input + start, i - start);
            add_token(tokens, get_token_type(word), word);
            free(word);
        }
    }
    return tokens;
}

// printf("word = %s\n", word);
// printf("strlen(word) = %lu\n", strlen(word));
// printf("input[i] = '%c' \n", input[i]);
// printf(" i_here = %d\n", i);