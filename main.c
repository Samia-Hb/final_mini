#include "minishell.h"

void print_ast(t_ast *ast, int depth)
{
    int i;

    i = 0;
    if (ast == NULL)
        return;
    while (i < depth)
    {
        printf("    ");
        i++;
    }
    if (ast->node && ast->node->token)
        printf("Token: %s (Type: %d)\n", ast->node->token->value, ast->node->token->type);
    else
        printf("NULL Token\n");
    print_ast(ast->left, depth + 1);
    print_ast(ast->right, depth + 1);
}

t_queue *push_queue(t_queue *queue, Token *node)
{
    t_queue *new_node;
    t_queue *temp;

    new_node = malloc(sizeof(t_queue));
    if (!new_node)
        return (NULL);
    new_node->node = node;
    new_node->next = NULL;
    if (!queue)
        return new_node;
    temp = queue;
    while (temp->next)
        temp = temp->next;
    temp->next = new_node;
    return queue;
}

// int get_precedence(int type)
// {

// 	if (type == TOKEN_OPEN_PARENTH || type == TOKEN_CLOSE_PARENTH)
// 		return (4);
// 	if (type == TOKEN_DOUBLE_AMP || type == TOKEN_DOUBLE_PIPE)
// 		return (3);
// 	if (type == TOKEN_PIPE)
// 		return (2);
// 	if (type == TOKEN_REDIR_APPEND || type == TOKEN_REDIR_HERE_DOC || type == TOKEN_REDIR_OUT
// 		|| type == TOKEN_REDIR_IN)
// 		return (1);
// 	return (0);
    ////////////////
    // if (token_type == TOKEN_WHITESPACES)
    //     return 25;
    // else if (token_type == TOKEN_NEW_LINE)
    //     return 2;
    // else if (token_type == TOKEN_COLON)
    //     return 3;
    // else if (token_type == TOKEN_SLASH)
    //     return 18;
    // else if (token_type == TOKEN_ASTERISK)
    //     return 19;
    // else if (token_type == TOKEN_EQUALS)
    //     return 13;
    // else if (token_type == TOKEN_PLUS)
    //     return 14;
    // else if (token_type == TOKEN_EXCLAMATION)
    //     return 20;
    // else if (token_type == TOKEN_AMPERSAND)
    //     return 12;
    // else if (token_type == TOKEN_DOUBLE_AMP)
    //     return 11;
    // else if (token_type == TOKEN_BACKTICK)
    //     return 24;
    // else if (token_type == TOKEN_TILDLE)
    //     return 22;
    // else if (token_type == TOKEN_SEMICOLON)
    //     return 4;
    // else if (token_type == TOKEN_BACKSLASH)
    //     return 17;
    // else if (token_type == TOKEN_DOT)
    //     return 15;
    // else if (token_type == TOKEN_QUESTION)
    //     return 16;
    // else if (token_type == TOKEN_MINUS)
    //     return 14;
    // else if (token_type == TOKEN_PIPE)
    //     return 10;
    // else if (token_type == TOKEN_REDIR_IN)
    //     return 8;
    // else if (token_type == TOKEN_DOUBLE_PIPE)
    //     return 8;
    // else if (token_type == TOKEN_REDIR_OUT)
    //     return 7;
    // else if (token_type == TOKEN_REDIR_APPEND)
    //     return 6;
    // else if (token_type == TOKEN_REDIR_HERE_DOC)
    //     return 5;
    // else if (token_type == TOKEN_DOLLAR)
    //     return 23;
    // else if (token_type == TOKEN_OPEN_PARENTH)
    //     return 21;
    // else if (token_type == TOKEN_CLOSE_PARENTH)
    //     return 21;
//     return -1;
// }

t_ast *push_to_ast_stack(t_ast *ast_stack, t_ast *ast_node)
{
    ast_node->next = ast_stack;
    return (ast_node);
}

t_ast *pop_ast_stack(t_ast **ast_stack)
{
    t_ast *head = *ast_stack;
    if (head)
        *ast_stack = head->next;
    return head;
}

void print_queue(t_queue *queue)
{
    while (queue)
    {
        printf("queue_data = %s\n", queue->node->value);
        queue = queue ->next;
    }
}

// int check_precedence(t_stack *stack, int token_type)
// {
// 	while (stack)
// 	{

// 		if (get_precedence(stack->node->type) >= get_precedence(token_type))
// 			return (0);
// 		stack = stack->next;
// 	}
// 	return (1);
// }

void print_tokens(Token *tokens)
{
    while (tokens)
    {
        printf("data = %s type = %d\n", (tokens)->value, (tokens)->type);
        (tokens) = (tokens)->next;
    }
}

int	main(void)
{
	char	*input;
	Token	**tokens;
    // t_queue *queue;
    // t_ast   *ast;
    int     errno;

	tokens = NULL;
    handle_signal();
	while (1)
	{
		input = readline("Minishell$ ");
        add_history(input);
		tokens = tokenize(input);
        analyse_tokens(tokens);
        expand(*tokens);
        
        // queue = generate_postfix(*tokens);
		// ast = generate_ast_from_postfix(queue);
		// print_ast(ast, 5);
	}
	return (0);
}