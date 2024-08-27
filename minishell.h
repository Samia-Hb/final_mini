#ifndef MINISHELL_H
#define MINISHELL_H

#include <readline/readline.h>
#include <readline/history.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <signal.h>
#include <unistd.h>

#define PREC_WHITESPACES          25  
#define PREC_BACKTICK             24
#define PREC_DOLLAR               23
#define PREC_TILDLE               22 
#define PREC_PARENTHESES          21 
#define PREC_LOGICAL_NOT          20 
#define PREC_ASTERISK             19 
#define PREC_SLASH                18
#define PREC_BACKSLASH            17 
#define PREC_QUESTION             16
#define PREC_DOT                  15
#define PREC_ADDITIVE             14
#define PREC_ASSIGNMENT           13
#define PREC_AMPERSAND            12
#define PREC_LOGICAL_AND          11 
#define PREC_PIPE                 9  // LOWER than redirection
#define PREC_LOGICAL_OR           8
#define PREC_REDIR_IN             10  // HIGHER than pipe
#define PREC_REDIR_OUT            10  // HIGHER than pipe
#define PREC_REDIR_APPEND         10  // HIGHER than pipe
#define PREC_REDIR_HERE_DOC       10  // HIGHER than pipe
#define PREC_SEMICOLON            4
#define PREC_COLON                3 
#define PREC_NEW_LINE             2


typedef enum
{
	TOKEN_WHITESPACES,    // (” “, “\t”, etc.
	TOKEN_NEW_LINE,       // "\n"
	TOKEN_COLON,          // ";"
	TOKEN_SLASH,          // "/"
	TOKEN_ASTERISK,       // "*" wildcard
	TOKEN_EQUALS,         // "="
	TOKEN_PLUS,           // "+"
	TOKEN_EXCLAMATION,    // "!"
	TOKEN_AMPERSAND,      // "&"
	TOKEN_DOUBLE_AMP,     // "&&"
	TOKEN_BACKTICK,       // "`"
	TOKEN_TILDLE,         // "~"
	TOKEN_SEMICOLON,      // ";"
	TOKEN_BACKSLASH,      // "\"
	TOKEN_DOT,            // "."
	TOKEN_QUESTION,       // "?""
	TOKEN_MINUS,          // "-"
	TOKEN_PIPE,           // "|"
	TOKEN_DOUBLE_PIPE,    // "||"
	TOKEN_REDIR_IN,       //"<"
	TOKEN_REDIR_OUT,      //">"
	TOKEN_REDIR_APPEND,   // ">>"
	TOKEN_REDIR_HERE_DOC, // "<<"
	TOKEN_DOLLAR,         // "$"
	TOKEN_OPEN_PARENTH,   // "( , [  , {"
	TOKEN_CLOSE_PARENTH,  // ") , ] , ]"
	TOKEN_COMMAND,
	TOKEN_OPTION,
	TOKEN_ARGUMENT, // String
	TOKEN_UNKNOWN
}					TokenType;

typedef struct token
{
	TokenType		type;
	char			*value;
	bool			double_quoted;
	bool			sigle_quoted;
	struct token	*next;
	struct token	*previous;
}					Token;

typedef struct command
{
	char	*command;
	char	**arguments;
	int		stdin_file;
	int		stdout_file;
	char	*operator;
}t_parser;

typedef struct ASTNode
{
	Token *token;
	int				fd_in;
	int				fd_out;
	int				arg_nbr;
	struct ASTNode	*left;
	struct ASTNode	*right;
}					ASTNode;

typedef struct s_ast
{
	ASTNode			*node; 
	struct s_ast	*left;
	struct s_ast	*right;
	struct s_ast	*next;
}					t_ast;

typedef struct queue
{
	Token			*node;
	struct queue	*next;
}					t_queue;

typedef struct stack
{
	Token			*node;   // Changed to t_ast pointer
	struct stack	*next;
}					t_stack;

			//**Tokenization**/
Token	**tokenize(char *input);
void	add_token(Token **tokens, TokenType type, const char *value);
char	*handle_quote(char *str, char c);
void	print_token(Token *tokens);

			//**libft**/
char	**ft_split(char *s, char c);
char	*ft_strjoin(char const *s1, char const *s2);
char	*ft_strtrim(char *s1, char *set);
char	*ft_substr(char *s, unsigned int start, size_t len);

		    //pipex_utils
char	*get_executable(char *command);

			//parser
void	ft_parser(Token *tokens);

			//generate_postfix
t_queue *generate_postfix(Token *tokens);
int		get_precedence(int token_type);
void	transfer_tokens_to_stack(Token *token_list, t_stack **stack);
int		check_precedence(t_stack *stack, int token_type);
void	push_back_stack(t_stack **src, t_stack **dest);

			//abstract syntax tree
t_ast	*generate_ast_from_postfix(t_queue *postfix_output);
t_stack *pop_stack(t_stack **stack);
t_ast *pop_ast_stack(t_ast **ast_stack);
int		is_operator(Token *node);
int		is_operand(Token *node);
t_ast	*push_to_ast_stack(t_ast *ast_stack, t_ast *ast_node);

			//mini_utils
void	print_stack(t_stack *head);
t_stack *new_stack_node(Token *token);
void push_top_stack(t_stack **src, t_stack **dest);
int check_syntax_errors(Token *tokens);

#endif
