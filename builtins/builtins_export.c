/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_export.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omiskiny <omiskiny@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/10 14:59:47 by omiskiny          #+#    #+#             */
/*   Updated: 2026/06/10 15:00:09 by omiskiny         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	update_or_add_env(t_env **env_list, char *key, char *value)
{
	t_env	*tmp;
	char	*dup_value;

	tmp = *env_list;
	while (tmp)
	{
		if (ft_strncmp(tmp->key, key, ft_strlen(key) + 1) == 0)
		{
			if (value)
			{
				if (tmp->value)
					free(tmp->value);
				tmp->value = ft_strdup(value);
			}
			return ;
		}
		tmp = tmp->next;
	}
	dup_value = NULL;
	if (value)
		dup_value = ft_strdup(value);
	env_add_back(env_list, ft_strdup(key), dup_value);
}

static void	print_export(t_env *env_list)
{
	while (env_list)
	{
		ft_putstr_fd("declare -x ", 1);
		ft_putstr_fd(env_list->key, 1);
		if (env_list->value)
		{
			write(1, "=\"", 2);
			ft_putstr_fd(env_list->value, 1);
			write(1, "\"", 1);
		}
		write(1, "\n", 1);
		env_list = env_list->next;
	}
}

static void	process_export_arg(char *arg, t_env **env_list)
{
	char	*ptr;
	char	*key;

	if (ft_strchr(arg, '='))
	{
		ptr = ft_strchr(arg, '=');
		key = ft_substr(arg, 0, ptr - arg);
		if (!key)
			return ;
		update_or_add_env(env_list, key, ptr + 1);
		free(key);
	}
	else
		update_or_add_env(env_list, arg, NULL);
}

void	builtin_export(char **args, t_env **env_list, t_shell *sh)
{
	int		i;

	i = 1;
	if (!args[i])
	{
		print_export(*env_list);
		return ;
	}
	while (args[i])
	{
		if (!is_valid_identifier(args[i]))
		{
			ft_putstr_fd("minishell: export: `", 2);
			ft_putstr_fd(args[i], 2);
			ft_putstr_fd("': not a valid identifier\n", 2);
			sh->exit_code = 1;
		}
		else
			process_export_arg(args[i], env_list);
		i++;
	}
}
