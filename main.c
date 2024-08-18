#include "minishell.h"


void print_ast(t_ast *node)
{
    if (node == NULL)
        return;
    printf("data = %s type = %d\n", node->node->token->value, node->node->token->type);
    print_ast(node->left);
    print_ast(node->right);
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

int get_precedence(int token_type)
{
    if (token_type == TOKEN_WHITESPACES)
        return 25;
    else if (token_type == TOKEN_NEW_LINE)
        return 2;
    else if (token_type == TOKEN_COLON)
        return 3;
    else if (token_type == TOKEN_SLASH)
        return 18;
    else if (token_type == TOKEN_ASTERISK)
        return 19;
    else if (token_type == TOKEN_EQUALS)
        return 13;
    else if (token_type == TOKEN_PLUS)
        return 14;
    else if (token_type == TOKEN_EXCLAMATION)
        return 20;
    else if (token_type == TOKEN_AMPERSAND)
        return 12;
    else if (token_type == TOKEN_DOUBLE_AMP)
        return 11;
    else if (token_type == TOKEN_BACKTICK)
        return 24;
    else if (token_type == TOKEN_TILDLE)
        return 22;
    else if (token_type == TOKEN_SEMICOLON)
        return 4;
    else if (token_type == TOKEN_BACKSLASH)
        return 17;
    else if (token_type == TOKEN_DOT)
        return 15;
    else if (token_type == TOKEN_QUESTION)
        return 16;
    else if (token_type == TOKEN_MINUS)
        return 14; 
    else if (token_type == TOKEN_PIPE)
        return 10;
    else if (token_type == TOKEN_DOUBLE_PIPE)
        return 9;
    else if (token_type == TOKEN_REDIR_IN)
        return 8;
    else if (token_type == TOKEN_REDIR_OUT)
        return 7;
    else if (token_type == TOKEN_REDIR_APPEND)
        return 6;
    else if (token_type == TOKEN_REDIR_HERE_DOC)
        return 5;
    else if (token_type == TOKEN_DOLLAR)
        return 23;
    else if (token_type == TOKEN_OPEN_PARENTH)
        return 21;
    else if (token_type == TOKEN_CLOSE_PARENTH)
        return 21;
    return -1;
}

int is_operator(Token *node)
{
    if (node->type == TOKEN_COMMAND || node->type == TOKEN_OPTION || node->type == TOKEN_UNKNOWN || node->type == TOKEN_ARGUMENT)
        return (0);
    return (1);
}

int is_operand(Token *node)
{
    if (node->type == TOKEN_COMMAND || node->type == TOKEN_OPTION || node->type == TOKEN_UNKNOWN || node->type == TOKEN_ARGUMENT)
        return (1);
    return (0);
}

t_stack *push_stack(t_stack *stack, Token *node)
{
	t_stack *head;

	head = malloc(sizeof(t_stack));
	if (!head)
		return (NULL);
	head->node = node;
	head->next = stack;
	return head;
}

t_stack *pop_stack(t_stack **stack)
{
    t_stack *top; 
    t_stack *node;
    if (!stack || !*stack)
        return NULL;
    top = (*stack);
    *stack = top->next;
    node = (*stack)->next;
    return (top);
}

t_ast *create_ast_node(Token *token)
{
    // Allocate memory for the t_ast structure
    t_ast *new_node = malloc(sizeof(t_ast));
    if (!new_node)
        return NULL;

    // Allocate memory for the ASTNode structure
    new_node->node = malloc(sizeof(ASTNode));
    if (!new_node->node)
    {
        free(new_node);
        return NULL;
    }

    // Allocate memory for the Token structure
    new_node->node->token = malloc(sizeof(Token)); // <-- This is the missing allocation
    if (!new_node->node->token)
    {
        free(new_node->node);
        free(new_node);
        return NULL;
    }

    // Duplicate the token's value and assign the type
    new_node->node->token->value = strdup(token->value);
    new_node->node->token->type = token->type;

    // Initialize the rest of the ASTNode and t_ast pointers
    new_node->node->left = NULL;
    new_node->node->right = NULL;
    new_node->left = NULL;
    new_node->right = NULL;
    new_node->next = NULL;

    return new_node;
}


// t_ast *create_ast_node(Token *token)
// {
//     t_ast *new_node = malloc(sizeof(t_ast));
//     if (!new_node)
//         return (NULL);
//     new_node->node = malloc(sizeof(ASTNode));
//     if (!new_node->node)
//     {
//         free (new_node);
//         return (NULL);
//     }
//     new_node->node->token->value = strdup(token->value);
//     new_node->node->token->type = token->type;
//     new_node->node->left = NULL;
//     new_node->node->right = NULL;
//     new_node->left = NULL;
//     new_node->right = NULL;
//     new_node->next = NULL;
//     return (new_node);
// }

t_ast *push_to_ast_stack(t_ast *ast_stack, t_ast *ast_node)
{
    ast_node->next = ast_stack;
    return ast_node;
}

t_ast *pop_ast_stack(t_ast **ast_stack)
{
    t_ast *head = *ast_stack;
    if (head)
        *ast_stack = head->next;
    return head;
}

int check_precedence(t_stack *stack, int token_type)
{
	while (stack)
	{
		if (get_precedence(stack->node->type) > get_precedence(token_type))
			return (0);
		stack = stack->next;
	}
	return (1);
}

t_queue *generate_postfix(Token *tokens)
{
    t_queue *output_queue = NULL;
    t_stack *holding_stack = NULL;
    t_stack *tmp = NULL;

    while (tokens)
    {
        if (tokens->type == TOKEN_COMMAND || tokens->type == TOKEN_UNKNOWN ||
            tokens->type == TOKEN_OPTION || tokens->type == TOKEN_ARGUMENT)
        {
            output_queue = push_queue(output_queue, tokens);
        }
        else
        {
            if (!holding_stack)
                holding_stack = push_stack(holding_stack, tokens);
            else
            {
                while (holding_stack && !check_precedence(holding_stack, tokens->type))
                {
                    tmp = pop_stack(&holding_stack);
                    output_queue = push_queue(output_queue, tmp->node);
                    holding_stack = holding_stack->next;
                }
                holding_stack = push_stack(holding_stack, tokens);
            }
        }
        tokens = tokens->next;
    }
    while (holding_stack)
    {
        output_queue = push_queue(output_queue, holding_stack->node);
        holding_stack = holding_stack->next;
    }
    return (output_queue);
}
// cat file.txt | sort | uniq > output.txt

t_ast *generate_ast_from_postfix(Token *tokens)
{
    t_ast	*ast_stack;
    t_queue	*postfix_queue;
	
    ast_stack = NULL;
	postfix_queue = generate_postfix(tokens);
    while (postfix_queue)
    {
        if (is_operand(postfix_queue->node))
        {
            t_ast *new_node = create_ast_node(postfix_queue->node);
            ast_stack = push_to_ast_stack(ast_stack, new_node);
		}
        else if (is_operator(postfix_queue->node))
        {
            t_ast *right = pop_ast_stack(&ast_stack);
            t_ast *left = pop_ast_stack(&ast_stack);
            t_ast *new_node = create_ast_node(postfix_queue->node);
            new_node->node->left = left->node;
            new_node->node->right = right->node;
            ast_stack = push_to_ast_stack(ast_stack, new_node);
        }
        postfix_queue = postfix_queue->next;
    }
    return pop_ast_stack(&ast_stack);
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
	// Token	*tmp;
	Token	**tokens;
    t_queue *postfix_queue;
	tokens = NULL;
	while (1)
	{
		input = readline("Minishell$ ");
		tokens = tokenize(input);
        postfix_queue = generate_postfix(*tokens);
        // print_tokens(*tokens);
		t_ast *ast = generate_ast_from_postfix(*tokens);
		print_ast(ast);
	}
	return (0);
}

		// while(postfix_queue)
		// {
		//     printf("data = %s type = %d\n", postfix_queue->node->value, postfix_queue->node->type);
		//     postfix_queue = postfix_queue->next;
		// }