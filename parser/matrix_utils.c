/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   matrix_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omiskiny <omiskiny@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/10 16:49:18 by omiskiny          #+#    #+#             */
/*   Updated: 2026/06/10 16:49:35 by omiskiny         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	matrix_len(char **matrix)
{
	int	i;

	i = 0;
	if (!matrix)
		return (0);
	while (matrix[i])
		i++;
	return (i);
}

static char	**copy_matrix(char **args, char *new_arg, int len)
{
	char	**new_matrix;
	int		i;

	new_matrix = malloc(sizeof(char *) * (len + 2));
	if (!new_matrix)
		return (NULL);
	i = 0;
	while (i < len)
	{
		new_matrix[i] = args[i];
		i++;
	}
	new_matrix[i] = ft_strdup(new_arg);
	if (!new_matrix[i])
	{
		free(new_matrix);
		return (NULL);
	}
	new_matrix[i + 1] = NULL;
	return (new_matrix);
}

char	**add_arg_to_matrix(char **args, char *new_arg)
{
	char	**new_matrix;
	int		len;

	if (!new_arg || new_arg[0] == '\0')
	{
		free(new_arg);
		return (args);
	}
	len = matrix_len(args);
	new_matrix = copy_matrix(args, new_arg, len);
	if (args)
		free(args);
	return (new_matrix);
}
