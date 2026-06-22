NAME = minishell

CC = cc
CFLAGS = -Wall -Wextra -Werror

# Автоматическое определение ОС (Mac или Linux/Windows WSL)
UNAME_S := $(shell uname -s)

ifeq ($(UNAME_S), Darwin)
	# Настройки для macOS (процессоры Apple Silicon)
	CFLAGS  += -I/opt/homebrew/opt/readline/include
	LDFLAGS = -L/opt/homebrew/opt/readline/lib -lreadline
else
	# Настройки для Windows WSL / Linux (readline обычно предустановлен в системе)
	LDFLAGS = -lreadline
endif

LIBFT_DIR = ./libft
LIBFT = $(LIBFT_DIR)/libft.a

SRCS = \
	main.c \
	lexer/lexer.c \
	lexer/helper.c \
	syntax/syntax_check.c \
	syntax/parenthesis_check.c \
	parser/parser.c \
	parser/parser_helpers.c \
	parser/print_parsed.c \
	expander/expand.c \
	expander/expand_string.c \
	expander/env_search.c \
	expander/expand_utils.c \
	expander/ft_strljoin.c \
	error.c \
	execution/exec_cmd.c \
	execution/executor.c \
	system/redirections.c \
	system/heredoc.c \
	system/signals.c \
	system/path.c \
	builtins/builtins_check.c \
	builtins/builtins_utils.c \
	builtins/builtins.c \
	builtins/env.c \
	builtins/env_utils.c \
	utils/free_cmd.c

OBJS = $(SRCS:.c=.o)

INCLUDES = -I$(LIBFT_DIR) -Iincludes

all: $(NAME)

$(NAME): $(LIBFT) $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) $(LIBFT) $(LDFLAGS) -o $(NAME)

$(LIBFT):
	$(MAKE) -C $(LIBFT_DIR)

%.o: %.c
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

clean:
	$(MAKE) -C $(LIBFT_DIR) clean
	rm -f $(OBJS)

fclean: clean
	$(MAKE) -C $(LIBFT_DIR) fclean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
