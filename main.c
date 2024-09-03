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
    t_parser *parse;

    int i;
	tokens = NULL;
    handle_signal();
	while (1)
	{
		input = readline("Minishell$ ");
        add_history(input);
		tokens = tokenize(input);
        errno = check_syntax_errors(*tokens);
        if (errno)
            exit(errno);
        // expand(*tokens);
        parse = analyse_tokens(tokens);
        while (parse)
        {
            i = 0;
            printf("command = %s\n", parse->token->value);
            while(parse->arguments[i])
            {
                printf("---->argument %d = %s\n", i,parse->arguments[i]->value);
                i++;
            }
            parse = parse->next;
        }
        exit(1);
        // queue = generate_postfix(*tokens);
		// ast = generate_ast_from_postfix(queue);
		// print_ast(ast, 5);
	}
	return (0);
}