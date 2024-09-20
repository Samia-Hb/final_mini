/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shebaz <shebaz@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/20 14:19:52 by shebaz            #+#    #+#             */
/*   Updated: 2024/09/20 23:20:07 by shebaz           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*get_word_to_expand(char *str)
{
	char	*word;
	int		length;
	int		i;

	i = 0;
	length = 0;
	while (str[i] && ((str[i] <= 'z' && str[i] >= 'a') || (str[i] <= 'Z' && str[i] >= 'A') || (str[i] >= '0' && str[i] <= '9')))
	{
		i++;
		length++;
	}
	word = malloc(length + 1);
	if (!word)
		return (NULL);
	i = 0;
	while (i < length)
	{
		word[i] = str[i];
		i++;
	}
	word[i] = '\0';
	return (word);
}

char	*tidle_expansion(int *i)
{
	char	*result;

	result = getenv("HOME");
	if (!result)
		return (strdup(""));
	*i += 1;
	return (strdup(result));
}

char	*dollar_expand(char *input, int *i)
{
	char	*word;
	char	*result;

	word = get_word_to_expand(input + *i + 1);
	if (!word)
		return (strdup(""));
	result = getenv(word);
	if (!result)
		result = strdup("");
	*i += strlen(word) + 1;
	free(word);
	return (result);
}

char	*double_quote_expansion(char *input, int *i)
{
	char	*expanded_value;

	expanded_value = strdup("");
	if (!expanded_value)
		return (NULL);
	if (input[*i] == '"')
		(*i)++;
	while (input[*i] && input[*i] != '"')
	{
		if (input[*i] == '$')
			expanded_value = ft_strjoin(expanded_value, dollar_expand(input,
						i));
		else
		{
			expanded_value = ft_strjoin(expanded_value,
					char_to_string(input[*i], 0));
			(*i)++;
		}
	}
	if (input[*i] == '"')
		(*i)++;
	return (expanded_value);
}

char	*single_quote_expansion(char *input, int *i)
{
	char	*expanded_value;

	expanded_value = strdup("");
	if (!expanded_value)
		return (NULL);
	if (input[*i] == '\'')
		(*i)++;
	while (input[*i] && input[*i] != '\'')
	{
		expanded_value = ft_strjoin(expanded_value, char_to_string(input[*i],
					0));
		(*i)++;
	}
	if (input[*i] == '\'')
		(*i)++;
	return (expanded_value);
}

char	**join_arr(char **dest, char **src)
{
	char	**result;
	int		i;
	int		j;
	int		k;

	j = 0;
	i = 0;
	k = 0;
	while (dest && dest[i])
		i++;
	while (src && src[j])
		j++;
	result = malloc((i + j + 1) * sizeof(char *));
	if (!result)
		return (NULL);
	while (k < j)
	{
		result[i + k] = strdup(src[k]);
		k++;
	}
	if (i + j == 0)
	{ 
		result[0] = strdup("");
		result[1] = NULL; 
	}
	else
		result[i + j] = NULL;
	return (result);
}

char	**into_arr(char *input)
{
	char	**arr;

	arr = malloc(2 * sizeof(char *));
	if (!arr)
		return (NULL);
	arr[0] = input;
	arr[1] = NULL;
	return (arr);
}

void	expand(Token *tokens)
{
	char	*result;
	int		i;

	while (tokens)
	{
		i = 0;
		result = strdup("");
		if (!result)
			return ;
		tokens->expanded_value = NULL;
		while (tokens->value[i])
		{
			if (tokens->value[i] == '"')
				result = ft_strjoin(result,
						double_quote_expansion(tokens->value, &i));
			else if (tokens->value[i] == '\'')
				result = ft_strjoin(result,
						single_quote_expansion(tokens->value, &i));
			else if (tokens->value[i] == '~')
				result = ft_strjoin(result, tidle_expansion(&i));
			else if (tokens->value[i] == '$')
				result = ft_strjoin(result, dollar_expand(tokens->value, &i));
			else
			{
				result = ft_strjoin(result, char_to_string(tokens->value[i],
							0));
				i++;
			}
		}
		if (!strchr(result, '"') && !strchr (result, '\''))
			tokens->expanded_value = join_arr(tokens->expanded_value,
					ft_split(result, ' '));
		else
			tokens->expanded_value = join_arr(tokens->expanded_value,
					into_arr(result));
		free(result);
		tokens = tokens->next;
	}
}
