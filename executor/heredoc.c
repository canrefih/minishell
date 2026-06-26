/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omiskiny <omiskiny@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/10 15:13:40 by omiskiny          #+#    #+#             */
/*   Updated: 2026/06/25 15:24:04 by omiskiny         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	heredoc_sig_handler(int sig)
{
	(void)sig;
	write(1, "\n", 1);
	_exit(130);
}

static void	heredoc_loop(char *delim, int fd, t_hd_info info)
{
	char	*line;
	char	*expanded;

	while (1)
	{
		line = readline("> ");
		if (!line || (ft_strlen(line) == ft_strlen(delim)
				&& !ft_strncmp(line, delim, ft_strlen(delim) + 1)))
		{
			free(line);
			break ;
		}
		if (!info.is_q && ft_strchr(line, '$'))
		{
			expanded = expand_heredoc_line(line, info.env, info.sh);
			free(line);
			line = expanded;
		}
		ft_putendl_fd(line, fd);
		free(line);
	}
}

static void	run_heredoc_child(char *delim, char *filename, t_hd_info info)
{
	int	fd;

	if (ft_strlen(delim) > 0 && delim[ft_strlen(delim) - 1] == '\1')
	{
		info.is_q = 1;
		delim[ft_strlen(delim) - 1] = '\0';
	}
	fd = open(filename, O_CREAT | O_WRONLY | O_TRUNC, 0644);
	signal(SIGINT, heredoc_sig_handler);
	signal(SIGQUIT, SIG_DFL);
	heredoc_loop(delim, fd, info);
	close(fd);
	_exit(0);
}

static int	process_heredoc_node(t_token *r, int *id, t_env *env, t_shell *sh)
{
	char		*id_str;
	char		*delim;
	pid_t		pid;
	int			status;
	t_hd_info	info;

	delim = ft_strdup(r->next->value);
	id_str = ft_itoa((*id)++);
	if (r->heredoc_file)
		free(r->heredoc_file);
	r->heredoc_file = ft_strjoin(".heredoc_", id_str);
	free(id_str);
	(1) && (info.env = env, info.sh = sh, info.is_q = 0);
	pid = fork();
	if (pid == 0)
		run_heredoc_child(delim, r->heredoc_file, info);
	free(delim);
	waitpid(pid, &status, 0);
	if (WIFSIGNALED(status) && WTERMSIG(status) == SIGINT)
		return (sh->exit_code = 130, unlink(r->heredoc_file), 1);
	if (WIFEXITED(status) && WEXITSTATUS(status) != 0)
		return (sh->exit_code = WEXITSTATUS(status),
			unlink(r->heredoc_file), 1);
	return (0);
}

int	prepare_heredocs(t_cmd *cmds, t_env *env_list, t_shell *sh)
{
	t_cmd	*c;
	t_token	*r;
	int		id;

	c = cmds;
	id = 0;
	while (c)
	{
		r = c->redirects;
		while (r)
		{
			if (r->type == TOKEN_HEREDOC)
			{
				if (process_heredoc_node(r, &id, env_list, sh))
					return (1);
			}
			r = r->next;
		}
		c = c->next;
	}
	return (0);
}
