CFLAGS = -Wall -Wextra -Werror
CC = cc
LFLAGS=-lreadline
NAME = minishell

SRCS = main.c tokenization.c pipex_utils.c libft/ft_split.c libft/ft_strjoin.c libft/ft_strtrim.c
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