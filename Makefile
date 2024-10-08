CFLAGS = -Wall -Wextra -Werror
CC = cc
LFLAGS =-lreadline
NAME = minishell

SRCS = main.c expand_utils.c expand.c analyse_tokens.c token_into_stack.c signal_handle.c \
		postfix_to_tree.c minishell_utils.c tokenization.c infix_to_postfix.c \
		pipex_utils.c parser.c libft/ft_split.c libft/ft_strjoin.c libft/ft_strtrim.c \
		gnl/get_next_line.c gnl/get_next_line_utils.c
OBJS = $(SRCS:.c=.o)

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) -o $(NAME) $(OBJS) $(LFLAGS)

%.o:%.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJS)

fclean: clean
	rm -rf $(NAME)

re: clean all