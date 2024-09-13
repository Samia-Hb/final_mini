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

void expand_single_quote(Token *token)
{
    int		i;

    i = 1;
    while (token->value[i])
    {
		if (token->value[i] == '\'')
        	i++;
	    token->expanded_value = ft_strjoin(token->expanded_value, char_to_string(token->value[i], 0));
	    i++;
    }
}

void expand_double_quote(Token *token)
{
    int     i;
	char    *word;

    i = 0;
    while (token->value[i])
    {
        if (token->value[i] == '"')
            i++;
        if (token->value[i] == '$')
        {
            word = get_word_to_expand(token->value + i + 1);
            if (get_token_type(word, 0) == TOKEN_COMMAND)
            {
                printf("Permission denied.\n");
                exit(1);
            }
            if (!getenv(word))
                token->expanded_value = NULL;
            else
                token->expanded_value = ft_strjoin(token->expanded_value, getenv(word));
            i += strlen(word) + 2;
        }
        token->expanded_value = ft_strjoin(token->expanded_value, char_to_string(token->value[i], 0));
        i++;
    }
}

// char *expand_dollar(char *input)
// {
//     int     i;
//     char    *str;

//     i = 0;
//      while (!ft_is_seperator(input[i]))
//          i++;
//      str = strndup(input, i);
//     input + i;
//     return (getenv(str));
// }

// int check_around(char *value, int pos)
// {
//     int i;

//     i = 0;
//     while (i < pos)
//     {
//         if (value[i] == "'")
//     }
// }

void expand(Token *tokens)
{
    Token	*temp;
    int		i;

    temp = tokens;
    i = 0;
    while (temp)
    {
        temp->expanded_value = NULL;
        if (temp->type == TOKEN_DOUBLE_QUOTED)
			expand_double_quote(temp);
        else if (temp->type == TOKEN_SINGLE_QUOTED)
            expand_single_quote(temp);
        else
            expand_thing(temp);
        temp = temp->next;
    }
}



void expand(Token *tokens)
{
    Token   *temp;
    int     i;
    int     flag;

    temp = tokens;
    i = 0;
    flag = 0;
    while (temp)
    {
        while (temp->value[i])
        {
            if (temp->value[i] == '"')
                flag++;
            if (temp->value[i] == '\'')
                flag++;
            if (temp->value[i] == '$' && !(flag % 2))
            {

            }
            temp->expanded_value = ft_strjoin(temp->expanded_value, char_to_string(temp->value[i], 0));
        }
        temp = temp->next;
    }
}
