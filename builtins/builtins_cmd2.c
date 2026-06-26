/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_cmd2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omiskiny <omiskiny@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/10 14:51:42 by omiskiny          #+#    #+#             */
/*   Updated: 2026/06/10 14:58:40 by omiskiny         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	is_dash(char *arg)
{
	return (arg && arg[0] == '-' && arg[1] == '\0');
}

static void	handle_cd_dash(t_env **env_list, t_shell *sh)
{
	char	*oldpwd;
	char	*pwd;
	char	cwd[1024];

	oldpwd = ft_getenv(*env_list, "OLDPWD");
	pwd = ft_getenv(*env_list, "PWD");
	if (!oldpwd)
	{
		write(2, "minishell: cd: OLDPWD not set\n", 31);
		sh->exit_code = 1;
		return ;
	}
	write(1, oldpwd, strlen(oldpwd));
	write(1, "\n", 1);
	if (chdir(oldpwd) < 0)
	{
		perror("minishell: cd");
		sh->exit_code = 1;
		return ;
	}
	if (pwd)
		mini_setenv(env_list, "OLDPWD", pwd);
	if (getcwd(cwd, sizeof(cwd)) != NULL)
		mini_setenv(env_list, "PWD", cwd);
	sh->exit_code = 0;
}

static char	*resolve_cd_path(t_cmd *cmd, t_env *env_list)
{
	char	*home;

	if (!cmd->args[1])
	{
		home = ft_getenv(env_list, "HOME");
		if (!home || *home == '\0')
		{
			write(2, "minishell: cd: HOME not set\n", 28);
			return (NULL);
		}
		return (home);
	}
	if (cmd->args[1][0] == '~' && cmd->args[1][1] == '\0')
	{
		home = ft_getenv(env_list, "HOME");
		if (!home)
			return (NULL);
		return (home);
	}
	return (cmd->args[1]);
}

static int	execute_chdir(char *path, t_shell *sh)
{
	if (chdir(path) < 0)
	{
		ft_putstr_fd("minishell: cd: ", 2);
		ft_putstr_fd(path, 2);
		ft_putstr_fd(": ", 2);
		ft_putstr_fd(strerror(errno), 2);
		ft_putstr_fd("\n", 2);
		sh->exit_code = 1;
		return (0);
	}
	return (1);
}

void	builtin_cd(t_cmd *cmd, t_env **env_list, t_shell *sh)
{
	char	*path;
	char	cwd[1024];

	if (cmd->args[1] && cmd->args[2])
	{
		ft_putstr_fd("minishell: cd: too many arguments\n", 2);
		return ((void)(sh->exit_code = 1));
	}
	if (is_dash(cmd->args[1]))
	{
		handle_cd_dash(env_list, sh);
		return ;
	}
	path = resolve_cd_path(cmd, *env_list);
	if (!path || getcwd(cwd, sizeof(cwd)) == NULL)
		return ((void)(sh->exit_code = 1));
	if (!execute_chdir(path, sh))
		return ;
	mini_setenv(env_list, "OLDPWD", cwd);
	if (getcwd(cwd, sizeof(cwd)) != NULL)
		mini_setenv(env_list, "PWD", cwd);
	sh->exit_code = 0;
}
