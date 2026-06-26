/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omiskiny <omiskiny@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/10 16:49:56 by omiskiny          #+#    #+#             */
/*   Updated: 2026/06/10 16:50:28 by omiskiny         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_matrix(char **matrix)
{
	int	i;

	i = 0;
	if (!matrix)
		return ;
	while (matrix[i])
	{
		free(matrix[i]);
		i++;
	}
	free(matrix);
}

void	free_cmds(t_cmd *cmd_list)
{
	t_cmd	*tmp;
	int		i;

	while (cmd_list)
	{
		tmp = cmd_list->next;
		if (cmd_list->args)
		{
			i = 0;
			while (cmd_list->args[i])
			{
				free(cmd_list->args[i]);
				i++;
			}
			free(cmd_list->args);
		}
		if (cmd_list->redirects)
			free_tokens(cmd_list->redirects);
		free(cmd_list);
		cmd_list = tmp;
	}
}
