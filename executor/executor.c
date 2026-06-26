/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omiskiny <omiskiny@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/10 15:18:36 by omiskiny          #+#    #+#             */
/*   Updated: 2026/06/25 15:18:04 by omiskiny         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	handle_child_execution(t_cmd *curr, t_exec_ctx *ctx, t_shell *sh)
{
	char	**env_matrix;

	setup_child_pipes(ctx->prev_fd, ctx->fd, curr);
	if (apply_redirects(curr, sh) != 0)
	{
		free_cmds(ctx->cmds);
		_exit(sh->exit_code);
	}
	env_matrix = convert_env_list_to_matrix(*(ctx->env_list));
	run_child_process(curr, env_matrix, ctx->env_list, sh);
}

static int	execute_pipeline_node(t_cmd *curr, t_exec_ctx *ctx, t_shell *sh)
{
	int		fd[2];
	pid_t	pid;

	if (curr->next && pipe(fd) == -1)
		return (1);
	if (!curr->next && ctx->prev_fd == -1 && curr->args
		&& is_builtin(curr->args[0]))
		return (run_parent_builtin(curr, ctx->env_list, sh));
	pid = fork();
	if (pid == 0)
	{
		ctx->fd = fd;
		handle_child_execution(curr, ctx, sh);
	}
	*(ctx->last_pid) = pid;
	if (ctx->prev_fd != -1)
		close(ctx->prev_fd);
	if (curr->next)
	{
		close(fd[1]);
		ctx->prev_fd = fd[0];
	}
	return (0);
}

static void	update_exit_code(int status, pid_t pid, pid_t last_pid, t_shell *sh)
{
	if (pid == last_pid)
	{
		if (WIFEXITED(status))
			sh->exit_code = WEXITSTATUS(status);
		else if (WIFSIGNALED(status))
		{
			sh->exit_code = 128 + WTERMSIG(status);
			if (WTERMSIG(status) == SIGQUIT)
				write(2, "Quit (core dumped)\n", 20);
		}
	}
}

static void	wait_for_children(t_cmd *cmds, pid_t last_pid, t_shell *sh)
{
	int		status;
	pid_t	ret_pid;

	(void)cmds;
	status = 0;
	ret_pid = waitpid(-1, &status, 0);
	while (ret_pid > 0)
	{
		update_exit_code(status, ret_pid, last_pid, sh);
		ret_pid = waitpid(-1, &status, 0);
	}
}

int	executor_build(t_cmd *cmds, t_env **env_list, t_shell *sh)
{
	t_cmd		*curr;
	t_exec_ctx	ctx;
	pid_t		last_pid;

	if (!cmds)
		return (0);
	if (prepare_heredocs(cmds, *env_list, sh))
		return (sh->exit_code);
	ctx.cmds = cmds;
	ctx.env_list = env_list;
	ctx.prev_fd = -1;
	ctx.last_pid = &last_pid;
	curr = cmds;
	while (curr)
	{
		execute_pipeline_node(curr, &ctx, sh);
		curr = curr->next;
	}
	if (ctx.prev_fd != -1)
		close(ctx.prev_fd);
	wait_for_children(cmds, last_pid, sh);
	free_heredocs(cmds);
	return (sh->exit_code);
}
