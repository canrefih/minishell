/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_env.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omiskiny <omiskiny@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/10 14:53:04 by omiskiny          #+#    #+#             */
/*   Updated: 2026/06/10 15:03:19 by omiskiny         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	delete_env_node(t_env **env_list, t_env *curr, t_env *prev)
{
	if (prev == NULL)
		*env_list = curr->next;
	else
		prev->next = curr->next;
	if (curr->key)
		free(curr->key);
	if (curr->value)
		free(curr->value);
	free(curr);
}

void	builtin_unset(char **args, t_env **env_list)
{
	t_env	*curr;
	t_env	*prev;
	int		i;

	i = 0;
	while (args[++i])
	{
		curr = *env_list;
		prev = NULL;
		while (curr)
		{
			if (ft_strncmp(curr->key, args[i], ft_strlen(args[i]) + 1) == 0)
			{
				delete_env_node(env_list, curr, prev);
				break ;
			}
			prev = curr;
			curr = curr->next;
		}
	}
}

void	execute_builtin(t_cmd *cmd, t_env **env_list, t_shell *sh)
{
	if (!cmd->args || !cmd->args[0])
		return ;
	if (ft_strncmp(cmd->args[0], "pwd", 4) == 0)
		builtin_pwd();
	else if (ft_strncmp(cmd->args[0], "env", 4) == 0)
		builtin_env(*env_list);
	else if (ft_strncmp(cmd->args[0], "echo", 5) == 0)
		builtin_echo(cmd->args, sh);
	else if (ft_strncmp(cmd->args[0], "cd", 3) == 0)
		builtin_cd(cmd, env_list, sh);
	else if (ft_strncmp(cmd->args[0], "export", 7) == 0)
		builtin_export(cmd->args, env_list, sh);
	else if (ft_strncmp(cmd->args[0], "unset", 6) == 0)
		builtin_unset(cmd->args, env_list);
	else if (ft_strncmp(cmd->args[0], "exit", 5) == 0)
		builtin_exit(cmd->args, sh);
}
