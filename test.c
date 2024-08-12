#include "minishell.h"

void print_token(Token *tokens)
{
    if (!tokens)
		return;
	while (tokens)
    {
		printf("%s: %d\n", tokens->value ,tokens->type);
    	tokens = tokens->next;
	}
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
    if (!(strchr(str[i] , c)))
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

int	main(void)
{
	char	*input;
	while (1)
	{
		input = readline("Minishell$ ");
		char *word = handle_quote(input, 39);
		printf("%s\n", word);
	}
}