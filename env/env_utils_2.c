/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils_2.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omiskiny <omiskiny@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/10 17:02:38 by omiskiny          #+#    #+#             */
/*   Updated: 2026/06/10 17:02:53 by omiskiny         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*ft_getenv(t_env *env_list, char *key)
{
	int	n;

	while (env_list)
	{
		n = 0;
		while (env_list->key[n] && key[n] && env_list->key[n] == key[n])
			n++;
		if (env_list->key[n] == '\0' && key[n] == '\0')
			return (env_list->value);
		env_list = env_list->next;
	}
	return (NULL);
}

void	mini_setenv(t_env **env_list, char *key, char *value)
{
	t_env	*temp;
	char	*new_value;

	temp = *env_list;
	while (temp)
	{
		if (ft_strncmp(temp->key, key, ft_strlen(key) + 1) == 0)
		{
			new_value = mini_strdup(value);
			if (!new_value)
				return ;
			free(temp->value);
			temp->value = new_value;
			return ;
		}
		temp = temp->next;
	}
	env_add_back(env_list, mini_strdup(key), mini_strdup(value));
}
