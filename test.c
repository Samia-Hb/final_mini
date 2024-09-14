#include "minishell.h"

char *get_word_to_expand(char *str)
{
    int     i;
    char    *word;
    int     length;

    i = 0;
    length = 0;
    while (str[i] && str[i] != ' ' && str[i] != '\t' && str[i] != '"' && str[i] != '\'')
    {
        i++;
        length++;
    }
    word = malloc(length + 1); 
    i = 0;
    while(i < length)
    {
        word[i] = str[i]; 
        i++;
    }
    word[i] = '\0';
    return (word);
}

char *tidle_expansion(int *i)
{
	char *result;

	result = getenv("HOME");
	*i += strlen(result) + 1;
	return (result);
}

char *dollar_expand(char *input, int *i)
{
	char	*word;
	char	*result;

	word = get_word_to_expand(input);
	result = getenv(word);
	if (!result)
		result = strdup("");
	*i += strlen(word) + 1;
	return (result);
}

char *double_quote_expansion(char *input, int *i)
{
	char *expanded_value;

	expanded_value = NULL;
	if(input[*i] == '"')
		(*i)++;
	while (input[*i] && input[*i] != '"')
	{
		if (input[*i] == '$')
			expanded_value = ft_strjoin(expanded_value, dollar_expand(input + *i + 1, i));
		else
		{
			expanded_value = ft_strjoin(expanded_value, char_to_string(input[*i], 0));
			(*i)++;
		}
	}
	if (input[*i] == '"')
		(*i)++;
	return (expanded_value);
}

char *single_quote_expansion(char *input, int *i)
{
	char *expanded_value;

	expanded_value = NULL;
	if (input[*i] == '\'')
		(*i)++;
	while (input[*i] && input[*i] != '\'')
	{
		expanded_value = ft_strjoin(expanded_value, char_to_string(input[*i], 0));
		(*i)++;
	}
	if (input[*i] == '\'')
		(*i)++;
	return (expanded_value);
}

void expand(Token *tokens)
{
    Token   *temp;
    int     i;

    temp = tokens;
    while (tokens)
    {
        i = 0;
        tokens->expanded_value = NULL;
        while (tokens->value[i])
        {
            if (tokens->value[i] == '"')
				tokens->expanded_value = double_quote_expansion(tokens->value + i, &i);
            else if (tokens->value[i] == '\'')
				tokens->expanded_value = single_quote_expansion(tokens->value + i, &i);
			else if (tokens->value[i]  == '~')
				tokens->expanded_value = ft_strjoin(tokens->expanded_value, tidle_expansion(&i));
            else
            {
				if (tokens->value[i] == '$')
					tokens->expanded_value = ft_strjoin(tokens->expanded_value, dollar_expand(tokens->value + i + 1, &i));
                else
				{
					tokens->expanded_value = ft_strjoin(tokens->expanded_value, char_to_string(tokens->value[i], 0));
                	i++;
				}
            }
        }
        tokens = tokens->next;
    }
}