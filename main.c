#include "minishell.h"

void print_ast(t_ast *node)
{
	if (node == NULL)
		return;
	printf ("data = %s type = %d\n", node->node->value, node->node->type);
    print_ast(node->node->left);
    print_ast(node->node->right);
}
/////

// queue => FIFO
// stack => LIFO

typedef struct queue
{
	Token *node;
	struct queue *next;
}t_queue;

typedef struct stack
{
	Token *node;
	struct stack *next;
}t_stack;

t_stack *push_queue(t_stack *stack, Token *node)
{
	t_stack *head;

	head = malloc(sizeof(t_stack));
	if (!head)
		return (NULL);
	head->node = node;
	if (!stack)
		return (head);
	head->next = stack;
	stack = head;
	return (stack);
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


t_ast *generate_Ast(Token *tokens)
{
	t_queue *output_queue;
	t_stack *operator_stack;

	if (tokens->type == TOKEN_COMMAND || tokens->type == TOKEN_UNKNOWN
	|| tokens->type == TOKEN_OPTION || tokens->type == TOKEN_ARGUMENT)
    	output_queue = push_queue(output_queue, tokens);
    else if (tokens->type == TOKEN_OPEN_PARENTH)
    	operator_stack = push_stack(operator_stack, tokens);
    else if (tokens->type == TOKEN_CLOSE_PARENTH)
	{
	
	}
}


////
int	main(void)
{
	char	*input;
	Token	*tmp;
	Token	**tokens;

	tokens = NULL;
	while (1)
	{
		input = readline("Minishell$ ");
		tokens = tokenize(input);
		tmp = *tokens;
		while(tmp)
		{
			printf("token_value  = %s token_type = %d\n", tmp->value, tmp->type);
			tmp = tmp->next;
		}
		ft_parser (*tokens);
	}
	t_ast	*ast = generate_Ast(tokens);
	print_ast(ast);
	return (0);
}