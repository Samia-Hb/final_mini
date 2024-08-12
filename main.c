#include "minishell.h"

int	main(void)
{
	char	*input;
	Token	**tokens;
	Token	*current_token;

	tokens = NULL;
	while (1)
	{
		input = readline("Minishell$ ");
		tokens = tokenize(input);
		current_token = *tokens;
		print_token(*tokens);
	}

	// t_stack *operator_stack = NULL;
	// t_queue *output_queue = NULL;
	// while (current_token)
	// {
	//     if (current_token->type == COMMAND || current_token->type == ARGUMENT
		// || current_token->type == OPTION || current_token->type == NUMBER) {
	//         output_queue = push_queue(output_queue, current_token);
	//     } else if (current_token->type == OPEN_PARENTHESIS) {
	//         operator_stack = push_stack(operator_stack, current_token);
	//     } else if (current_token->type == CLOSE_PARENTHESIS) {
	//         while (peek_stack(operator_stack)
		// && peek_stack(operator_stack)->type != OPEN_PARENTHESIS) {
	//             output_queue = push_queue(output_queue,
			// pop_stack(&operator_stack));
	//         }
	//         pop_stack(&operator_stack);
	//     } else if (is_operator(current_token)) {
	//         while (peek_stack(operator_stack)
		// && is_operator(peek_stack(operator_stack)) &&
	//                get_precedence(peek_stack(operator_stack)) >= get_precedence(current_token)) {
	//             output_queue = push_queue(output_queue,
			// pop_stack(&operator_stack));
	//         }
	//         operator_stack = push_stack(operator_stack, current_token);
	//     }
	//     current_token = current_token->next;
	// }
	// while (peek_stack(operator_stack))
	// {
	//     output_queue = push_queue(output_queue, pop_stack(&operator_stack));
	// }
	// t_queue *queue;
	// queue = output_queue;
	// t_ast *ast_stack = NULL;
	// ASTNode *node;
	// while (queue)
	// {
	//     if (is_operator(queue->token))
	// 	{
	//         ASTNode *right;
	//         ASTNode *left;
	//         right = pop_ast(&ast_stack);
	//         left = pop_ast(&ast_stack);
	//         node = create_ast_node(queue->token->type, queue->token->value,
			// left, right);
	//         push_ast_stack(&ast_stack, node);
	//     } else
	// 	{
	//         node = create_ast_node(queue->token->type, queue->token->value,
			// NULL, NULL);
	//         push_ast_stack(&ast_stack, node);
	//     }
	//     queue = queue->next;
	// }
	// node = pop_ast(&ast_stack);
	// printTree(node, 0);
	// free_tokens(*tokens);
	// free(input);
	return (0);
}