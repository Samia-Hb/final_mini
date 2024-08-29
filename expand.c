#include "minishell.h"
// char get_nfs(char *expanded)
// {
//     int i;

//     i = 0;
//     while (expanded[i])
//     {
//          if(expanded[i])
//     }
// }

// void brace_expansion(Token **token)
// {
//     int i;

//     i = 0;
//     while ((*token)[i])
//     {
//         if((*token)[i] == '$')

//     }
// }

char *get_word_to_expand(char *str)
{
    int     i;
    char    *word;
    int     length;

    i = 0;
    length = 0;
    while (str[i] && str[i] != ' ' && str[i] != '\t')
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


char *expand_single_quote(Token *token)
{
    char	*expanded;
    int		i;
    int 	j;

	j = 0;
    i = 0;
    expanded = malloc(strlen(token->value) - 1);
    if (!expanded)
        return (NULL);
    while (token->value[i])
    {
		if (token->value[i] != '\'')
		{
        	expanded[j] = token->value[i];
			j++;
		}
		i++;  
    }
	expanded[j] = '\0';
	return (expanded);
}

void expand_quote(Token *token)
{
    int     i;
	int		type;
    char    *word;
    char    *str;

    i = 0;
    while (token->value[i])
    {
        if (token->value[i] == '$')
        {
            word = get_word_to_expand(token->value + i);
			type = get_word_type(word);
        }
    }
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
            temp->expanded_value = strdup(getenv("HOME"));
        else if(temp->type == TOKEN_SINGLE_QUOTED)
            temp->expanded_value = expand_single_quote(temp);
        else if(temp->type == TOKEN_DOUBLE_QUOTED)
            expand_double_quote(temp);
        else 
            temp->expanded_value = NULL;
        temp = temp->next;
    }
}
