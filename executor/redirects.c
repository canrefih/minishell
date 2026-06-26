/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirects.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omiskiny <omiskiny@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/10 15:22:23 by omiskiny          #+#    #+#             */
/*   Updated: 2026/06/25 14:58:13 by omiskiny         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	handle_input_redirection(t_token *tmp, int *fd_in)
{
	char	*clean_file;

	if (*fd_in != -1)
		close(*fd_in);
	if (tmp->type == TOKEN_HEREDOC)
		*fd_in = open(tmp->heredoc_file, O_RDONLY);
	else
	{
		if (!tmp->next)
			return (1);
		clean_file = clean_filename(tmp->next->value);
		*fd_in = open(clean_file, O_RDONLY);
		free(clean_file);
	}
	if (*fd_in < 0)
	{
		perror("minishell");
		return (1);
	}
	return (0);
}

static int	handle_output_redirection(t_token *tmp, int *fd_out)
{
	int		flags;
	char	*clean_file;

	if (*fd_out != -1)
		close(*fd_out);
	if (!tmp->next)
		return (1);
	if (tmp->type == TOKEN_RED_OUT)
		flags = O_CREAT | O_WRONLY | O_TRUNC;
	else
		flags = O_CREAT | O_WRONLY | O_APPEND;
	clean_file = clean_filename(tmp->next->value);
	*fd_out = open(clean_file, flags, 0644);
	free(clean_file);
	if (*fd_out < 0)
	{
		perror("minishell");
		return (1);
	}
	return (0);
}

static int	loop_redirects(t_token *tmp, int *fd_in, int *fd_out)
{
	while (tmp)
	{
		if (tmp->type == TOKEN_RED_IN || tmp->type == TOKEN_HEREDOC)
		{
			if (handle_input_redirection(tmp, fd_in))
				return (1);
		}
		else if (tmp->type == TOKEN_RED_OUT || tmp->type == TOKEN_APPEND)
		{
			if (handle_output_redirection(tmp, fd_out))
				return (1);
		}
		tmp = tmp->next;
	}
	return (0);
}

static int	apply_dup(int fd, int target)
{
	if (dup2(fd, target) < 0)
	{
		perror("dup2");
		return (1);
	}
	close(fd);
	return (0);
}

int	apply_redirects(t_cmd *cmd, t_shell *sh)
{
	int	fd[2];

	fd[0] = -1;
	fd[1] = -1;
	if (loop_redirects(cmd->redirects, &fd[0], &fd[1]))
		return ((void)(sh->exit_code = 1), 1);
	if (fd[0] != -1 && apply_dup(fd[0], STDIN_FILENO))
		return (1);
	if (fd[1] != -1 && apply_dup(fd[1], STDOUT_FILENO))
		return (1);
	return (0);
}
