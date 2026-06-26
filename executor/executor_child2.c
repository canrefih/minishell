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

static void	restore_io(int in, int out)
{
	dup2(in, STDIN_FILENO);
	dup2(out, STDOUT_FILENO);
	close(in);
	close(out);
}

int	run_parent_builtin(t_cmd *cmd, t_env **env_list, t_shell *sh)
{
	int	fd[2];

	fd[0] = dup(STDIN_FILENO);
	fd[1] = dup(STDOUT_FILENO);
	if (fd[0] < 0 || fd[1] < 0)
		return (1);
	if (apply_redirects(cmd, sh))
	{
		sh->exit_code = 1;
		restore_io(fd[0], fd[1]);
		return (sh->exit_code);
	}
	execute_builtin(cmd, env_list, sh);
	restore_io(fd[0], fd[1]);
	return (sh->exit_code);
}
