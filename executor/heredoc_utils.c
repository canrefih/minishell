/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omiskiny <omiskiny@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/10 15:18:36 by omiskiny          #+#    #+#             */
/*   Updated: 2026/06/15 11:47:57 by omiskiny         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*hd_join_and_free(char *res, char *to_append, int free_append)
{
	char	*tmp;

	tmp = ft_strjoin(res, to_append);
	free(res);
	if (free_append)
		free(to_append);
	return (tmp);
}

char	*expand_heredoc_line(char *line, t_env *env_list, t_shell *sh)
{
	char	*res;
	char	*expanded;
	int		i;
	char	c_str[2];

	res = ft_strdup("");
	i = 0;
	while (line[i])
	{
		if (line[i] == '$')
		{
			expanded = handle_dollar_expansion(line, &i, env_list, sh);
			res = hd_join_and_free(res, expanded, 1);
		}
		else
		{
			c_str[0] = line[i++];
			c_str[1] = '\0';
			res = hd_join_and_free(res, c_str, 0);
		}
	}
	return (res);
}

void	free_heredocs(t_cmd *cmds)
{
	t_cmd	*c;
	t_token	*r;

	c = cmds;
	while (c)
	{
		r = c->redirects;
		while (r)
		{
			if (r && r->heredoc_file)
			{
				unlink(r->heredoc_file);
				free(r->heredoc_file);
				r->heredoc_file = NULL;
			}
			r = r->next;
		}
		c = c->next;
	}
}
