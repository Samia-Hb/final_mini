#include "minishell.h"

void push_back_stack(t_stack **src, t_stack **dest)
{
	t_stack *node;
	t_stack *tmp;

    if (!src || !*src)
        return;
    node = pop_stack(src);
    if (!node)
        return;
    if (*dest == NULL)
        *dest = node;
    else
    {
        tmp = *dest;
        while (tmp->next)
            tmp = tmp->next;
        tmp->next = node;
    }
}

void push_top_stack(t_stack **src, t_stack **dest)
{
    if (!src || !*src)
        return;

    t_stack *node = pop_stack(src);
    if (!node)
        return;
    node->next = (*dest); 
    (*dest) = node;
}

void par_push(t_stack **head, t_stack **postfix_output, t_stack **holding_stack)
{
    t_stack *trash = NULL;
    push_top_stack(head, &trash);

    while ((*holding_stack) && (*holding_stack)->node->type != TOKEN_OPEN_PARENTH)
    {
        push_back_stack(holding_stack, postfix_output);
    }

    if (*holding_stack && (*holding_stack)->node->type == TOKEN_OPEN_PARENTH)
        push_top_stack(holding_stack, &trash); 
}

void tok_push(t_stack **head, t_stack **output_postfix, t_stack **holding_stack)
{
	if (!holding_stack)
		push_top_stack(head, holding_stack);
	else
	{
		while ((*holding_stack) && (!check_precedence(*holding_stack, (*head)->node->type)))
    	{
        	push_back_stack(holding_stack, output_postfix);
    	}
    	push_top_stack(head, holding_stack);
	}
}

void print_stack(t_stack *head)
{
	while (head)
	{
		printf(" %s ", head->node->value);
		head = head->next;
	}
	printf("\n");
}

int lst_size(t_stack *stack)
{
	int size;

	size = 0;
	while (stack)
	{
		size++;
		stack = stack->next;
	}
	return (size);
}

t_stack *generate_postfix(Token *tokens)
{
    t_stack *postfix_output;
    t_stack *holding_stack;
    t_stack *head;

	postfix_output = NULL;
	holding_stack = NULL;
	head = NULL;
    transfer_tokens_to_stack(tokens, &head);
    while (head)
    {
		// if (lst_size(postfix_output) == 2 && lst_size(holding_stack) == 1)
		// 	push_back_stack(&holding_stack, &postfix_output);
		if (head->node->type == TOKEN_COMMAND || head->node->type == TOKEN_UNKNOWN ||
			 head->node->type == TOKEN_OPTION || head->node->type == TOKEN_ARGUMENT)
        	push_back_stack(&head, &postfix_output);
		else if (head->node->type != TOKEN_CLOSE_PARENTH && head->node->type != TOKEN_OPEN_PARENTH)
		   	tok_push(&head, &postfix_output, &holding_stack);
        else if (head->node->type == TOKEN_CLOSE_PARENTH)
            push_top_stack(&head, &holding_stack);
        else if (head->node->type == TOKEN_OPEN_PARENTH)
            par_push(&head, &postfix_output, &holding_stack);
    }
    while (holding_stack)
	{
        push_back_stack(&holding_stack, &postfix_output);
	}
	return postfix_output;
}

