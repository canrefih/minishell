/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_child.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omiskiny <omiskiny@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/10 15:17:57 by omiskiny          #+#    #+#             */
/*   Updated: 2026/06/10 15:31:51 by omiskiny         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	handle_path_error(char *cmd, char *msg, int code, char **env)
{
	ft_putstr_fd("minishell: ", 2);
	ft_putstr_fd(cmd, 2);
	ft_putstr_fd(msg, 2);
	free_split(env);
	return (code);
}

static char	*check_absolute_path(char *cmd, char **env, int *status)
{
	struct stat	st;

	if (stat(cmd, &st) != 0)
	{
		*status = handle_path_error(cmd, ": No such file or directory\n",
				127, env);
		return (NULL);
	}
	if (S_ISDIR(st.st_mode))
	{
		*status = handle_path_error(cmd, ": Is a directory\n", 126, env);
		return (NULL);
	}
	if (access(cmd, X_OK) != 0)
	{
		*status = handle_path_error(cmd, ": Permission denied\n", 126, env);
		return (NULL);
	}
	return (ft_strdup(cmd));
}

int	validate_cmd_path(t_cmd *cmd, char **env, char **path, int *status)
{
	*path = NULL;
	if (!cmd->args[0] || cmd->args[0][0] == '\0')
		return (*status = 0, free_split(env), 0);
	if (ft_strchr(cmd->args[0], '/'))
	{
		*path = check_absolute_path(cmd->args[0], env, status);
		return (*path != NULL);
	}
	if (is_builtin(cmd->args[0]))
		return (*status = 0, 1);
	*path = find_cmd(cmd->args[0], env);
	if (!*path)
		return (*status = handle_path_error(cmd->args[0],
				": command not found\n", 127, env), 0);
	*status = 0;
	return (1);
}

void	run_child_process(t_cmd *cmd, char **env_matrix,
	t_env **env_list, t_shell *sh)
{
	char	*path;
	int		status;

	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	if (cmd && cmd->args && cmd->args[0] && is_builtin(cmd->args[0]))
	{
		execute_builtin(cmd, env_list, sh);
		free_split(env_matrix);
		_exit(0);
	}
	if (!validate_cmd_path(cmd, env_matrix, &path, &status))
		_exit(status);
	execve(path, cmd->args, env_matrix);
	perror(cmd->args[0]);
	free(path);
	free_split(env_matrix);
	_exit(126);
}

void	setup_child_pipes(int prev_fd, int fd[2], t_cmd *curr)
{
	if (prev_fd != -1)
	{
		dup2(prev_fd, STDIN_FILENO);
		close(prev_fd);
	}
	if (curr->next)
	{
		dup2(fd[1], STDOUT_FILENO);
		close(fd[0]);
		close(fd[1]);
	}
}
