/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils_2.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omiskiny <omiskiny@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/10 17:02:38 by omiskiny          #+#    #+#             */
/*   Updated: 2026/06/22 16:20:51 by omiskiny         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static t_env	*find_env_node(t_env *env_list, char *key)
{
	int	n;

	while (env_list)
	{
		n = 0;
		while (env_list->key[n] && key[n] && env_list->key[n] == key[n])
			n++;
		if (env_list->key[n] == '\0' && key[n] == '\0')
			return (env_list);
		env_list = env_list->next;
	}
	return (NULL);
}

static void	handle_shlvl(t_env **env_list)
{
	t_env	*shlvl_node;
	int		lvl;
	char	*lvl_str;

	shlvl_node = find_env_node(*env_list, "SHLVL");
	if (!shlvl_node)
		env_add_back(env_list, "SHLVL", "1");
	else
	{
		lvl = ft_atoi(shlvl_node->value);
		if (lvl < 0)
			lvl = 0;
		else
			lvl++;
		lvl_str = ft_itoa(lvl);
		free(shlvl_node->value);
		shlvl_node->value = mini_strdup(lvl_str);
		free(lvl_str);
	}
}

static int	fill_env_from_matrix(t_env **env_list, char **env_matrix)
{
	int		i;
	int		j;
	char	*key;
	char	*value;

	i = 0;
	while (env_matrix && env_matrix[i])
	{
		j = 0;
		while (env_matrix[i][j] && env_matrix[i][j] != '=')
			j++;
		key = malloc(sizeof(char) * (j + 1));
		if (!key)
			return (0);
		ft_strlcpy(key, env_matrix[i], j + 1);
		value = mini_strdup(&env_matrix[i][j + 1]);
		env_add_back(env_list, key, value);
		i++;
	}
	return (1);
}

t_env	*init_env_list(char **env_matrix)
{
	t_env	*env_list;
	char	cwd[1024];

	env_list = NULL;
	if (env_matrix && env_matrix[0] != NULL)
	{
		if (!fill_env_from_matrix(&env_list, env_matrix))
			return (NULL);
	}
	else
	{
		if (getcwd(cwd, sizeof(cwd)) != NULL)
			env_add_back(&env_list, "PWD", cwd);
		env_add_back(&env_list, "_", "./minishell");
		env_add_back(&env_list, "PATH",
			"/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin");
	}
	handle_shlvl(&env_list);
	return (env_list);
}
