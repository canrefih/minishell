# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: omiskiny <omiskiny@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2026/06/15 11:22:41 by omiskiny          #+#    #+#              #
#    Updated: 2026/06/25 14:52:52 by omiskiny         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME        = minishell
LIBFT_DIR   = ./libft
LIBFT       = $(LIBFT_DIR)/libft.a

SRC         = src/main.c \
              src/syntax_check.c \
              lexer/lexer_build.c \
              lexer/lexer_utils.c \
              parser/parser.c \
              parser/free_utils.c \
              parser/cmd_utils.c \
              parser/matrix_utils.c \
              env/env_utils_1.c \
              env/env_utils_2.c \
              env/env_utils_3.c \
              expander/expander.c \
              expander/expander_build.c \
              expander/expander_utils.c \
              expander/expander_dollar.c \
              executor/executor.c \
              executor/executor_utils.c \
              executor/executor_child.c \
              executor/executor_child2.c \
              executor/heredoc.c \
              executor/heredoc_utils.c \
              executor/redirect_utils.c \
              executor/redirects.c \
              builtins/builtins_export.c \
              builtins/builtins_utils.c \
              builtins/builtins_env.c \
              builtins/builtins_cmd1.c \
              builtins/builtins_cmd2.c \
              signals/signals.c

OBJS        = $(SRC:.c=.o)
HEADER      = minishell.h

CC          = cc
CFLAGS      = -Wall -Wextra -Werror -g -I. -I$(LIBFT_DIR)
LDFLAGS     = -lreadline
RM          = rm -f

GREEN       = \033[0;32m
YELLOW      = \033[0;33m
RESET       = \033[0m

all: $(LIBFT) $(NAME)

$(LIBFT):
	@echo "$(YELLOW)Compiling libft...$(RESET)"
	@$(MAKE) -C $(LIBFT_DIR)

$(NAME): $(OBJS) $(LIBFT)
	@echo "$(YELLOW)Compiling $(NAME)...$(RESET)"
	@$(CC) $(CFLAGS) $(OBJS) $(LIBFT) $(LDFLAGS) -o $(NAME)
	@echo "$(GREEN)Successfully created: $(NAME)$(RESET)"

%.o: %.c $(HEADER)
	@$(CC) $(CFLAGS) -c $< -o $@

clean:
	@$(MAKE) -C $(LIBFT_DIR) clean
	@$(RM) $(OBJS)
	@echo "$(YELLOW)Object files cleaned.$(RESET)"

fclean: clean
	@$(MAKE) -C $(LIBFT_DIR) fclean
	@$(RM) $(NAME)
	@echo "$(YELLOW)Program cleaned.$(RESET)"

re: fclean all

sanitize: fclean $(LIBFT)
	@echo "$(YELLOW)Compiling with AddressSanitizer...$(RESET)"
	@$(CC) $(CFLAGS) -fsanitize=address $(SRC) $(LIBFT) $(LDFLAGS) -o $(NAME)

val: all
	valgrind --leak-check=full --show-leak-kinds=all --track-fds=yes ./$(NAME)

.PHONY: all clean fclean re sanitize val