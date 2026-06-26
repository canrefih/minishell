/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omiskiny <omiskiny@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/15 11:23:02 by omiskiny          #+#    #+#             */
/*   Updated: 2026/06/25 15:23:16 by omiskiny         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <errno.h>
# include <string.h>
# include <sys/wait.h>
# include <sys/stat.h>
# include <sys/types.h>
# include <signal.h>
# include <fcntl.h>
# include <readline/readline.h>
# include <readline/history.h>
# include "libft.h"

extern int	g_signal;

typedef struct s_shell
{
	int	exit_code;
	int	should_exit;
}	t_shell;

typedef struct s_env
{
	char			*key;
	char			*value;
	struct s_env	*next;
}	t_env;

typedef struct s_hd_info
{
	t_env	*env;
	t_shell	*sh;
	int		is_q;
}	t_hd_info;

typedef enum e_token_type
{
	TOKEN_WORD,
	TOKEN_PIPE,
	TOKEN_RED_IN,
	TOKEN_RED_OUT,
	TOKEN_HEREDOC,
	TOKEN_APPEND
}	t_token_type;

typedef enum e_redir_mode
{
	MODE_PARENT,
	MODE_CHILD
}	t_redir_mode;

typedef struct s_token
{
	char			*value;
	t_token_type	type;
	struct s_token	*next;
	struct s_token	*prev;
	char			*heredoc_file;
	int				quoted;
}	t_token;

typedef struct s_cmd
{
	char			**args;
	t_token			*redirects;
	struct s_cmd	*next;
	struct s_cmd	*prev;
}	t_cmd;

typedef struct s_exec_ctx
{
	t_cmd			*cmds;
	t_env			**env_list;
	int				prev_fd;
	int				*fd;
	pid_t			*last_pid;
}	t_exec_ctx;

typedef struct s_expand_ctx
{
	char	*word;
	char	*res;
	t_env	*env_list;
	t_shell	*sh;
	int		i;
	int		sq;
	int		dq;
}	t_expand_ctx;

typedef struct s_expander_ctx
{
	t_cmd	*cmd;
	t_env	*env;
	t_shell	*sh;
	int		i;
	int		j;
	char	*old_val;
}	t_expander_ctx;

/* --- Signals --- */
void	init_signals(int mode);
void	handle_signals(int sig);
void	handle_signals_child(int sig);
void	handle_quit_child(int sig);

/* --- Lexer & Token Utils --- */
t_token	*create_token(char *value, t_token_type type);
void	token_add_back(t_token **lst, t_token *new_node);
t_token	*lexer_build(char *input);
void	free_tokens(t_token *lexer_list);
int		skip_spaces(char *str, int i);
int		is_operator(char c);

/* --- Syntax Check --- */
int		check_unclosed_quotes(char *input);
int		check_syntax_errors(t_token *tokens);

/* --- Parser --- */
t_cmd	*create_cmd_node(void);
void	cmd_add_back(t_cmd **lst, t_cmd *new_node);
char	**add_arg_to_matrix(char **args, char *new_arg);
t_cmd	*parser_build(t_token *tokens);
void	free_cmds(t_cmd *cmd_list);
void	free_matrix(char **matrix);

/* --- Environment Utils --- */
void	free_env_list(t_env *head);
t_env	*init_env_list(char **env_matrix);
char	*ft_getenv(t_env *env_list, char *key);
void	mini_setenv(t_env **env_list, char *key, char *value);
char	*mini_strdup(char *str);
void	env_add_back(t_env **lst, char *key, char *value);

/* --- Expander --- */
char	*expand_word(char *word, t_env *env_list, t_shell *sh);
void	expander_build(t_cmd *cmds, t_env *env_list, t_shell *sh);
void	expand_redirects(t_token *redir, t_env *env_list, t_shell *sh);
int		is_valid_env_char(char c);
char	*join_and_free(char *s1, char *s2);
char	*add_char_to_str(char *str, char c);
char	*handle_dollar_expansion(char *str, int *i,
			t_env *env_list, t_shell *sh);
char	*remove_quotes(char *str);

/* --- Executor --- */
int		executor_build(t_cmd *cmds, t_env **env_list, t_shell *sh);
void	free_split(char **arr);
char	*find_cmd(char *cmd, char **envp);
char	**convert_env_list_to_matrix(t_env *env_list);
void	run_child_process(t_cmd *cmd, char **env_matrix,
			t_env **env_list, t_shell *sh);
int		run_parent_builtin(t_cmd *cmd, t_env **env_list, t_shell *sh);
void	setup_child_pipes(int prev_fd, int fd[2], t_cmd *curr);
void	free_heredocs(t_cmd *cmds);
int		prepare_heredocs(t_cmd *cmds, t_env *env_list, t_shell *sh);
char	*expand_heredoc_line(char *line, t_env *env_list, t_shell *sh);
char	*clean_filename(char *str);
int		apply_redirects(t_cmd *cmd, t_shell *sh);

/* --- Builtins --- */
void	update_or_add_env(t_env **env_list, char *key, char *value);
void	builtin_export(char **args, t_env **env_list, t_shell *sh);
int		is_n_flag(char *str);
int		is_numeric(char *str);
int		is_valid_identifier(char *str);
int		is_builtin(char *cmd);
void	builtin_unset(char **args, t_env **env_list);
void	execute_builtin(t_cmd *cmd, t_env **env_list, t_shell *sh);
void	builtin_cd(t_cmd *cmd, t_env **env_list, t_shell *sh);
void	builtin_exit(char **args, t_shell *sh);
void	builtin_pwd(void);
void	builtin_env(t_env *env_list);
void	builtin_echo(char **args, t_shell *sh);

#endif
