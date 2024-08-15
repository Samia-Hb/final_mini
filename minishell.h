#ifndef MINISHELL_H
#define MINISHELL_H

#include <readline/readline.h>
#include <readline/history.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define PREC_PIPE 0
#define PREC_LOGICAL_AND 8
#define PREC_LOGICAL_OR 7
#define PREC_SEQUENTIAL_EXECUTION 2
#define PREC_ADDITIVE 15
#define PREC_MULTIPLICATIVE 16

char				*ft_strjoin(char const *s1, char const *s2);
char				*get_executable(char *command);


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

typedef struct ASTNode
{
	TokenType		type;
	char			*value;
	struct ASTNode	*left;
	struct ASTNode	*right;
}					ASTNode;

typedef struct s_ast
{
	ASTNode			*node;
	struct s_ast	*next;
}					t_ast;

typedef struct token
{
	TokenType		type;
	char			*value;
	struct token	*next;
}					Token;

typedef struct command
{
	char	*command;
	char	**arguments;
	int		infile;
	int		outfile;
}t_command;

typedef struct stack
{
	Token			*token;
	struct stack	*next;
}					t_stack;

typedef struct queue
{
	Token			*token;
	struct queue	*next;
}					t_queue;

			//**Tokenization**/
Token	**tokenize(char *input);
void add_token(Token **tokens, TokenType type, const char *value);
char	*handle_quote(char *str, char c);
void print_token(Token *tokens);

			//**libft**/
char	**ft_split(char *s, char c);
char	*ft_strjoin(char const *s1, char const *s2);
char	*ft_strtrim(char *s1, char *set);
char	*ft_substr(char *s, unsigned int start, size_t len);

		    //pipex_utils
char	*get_executable(char *command);



#endif