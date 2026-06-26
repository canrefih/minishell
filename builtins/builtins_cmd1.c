/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_cmd1.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omiskiny <omiskiny@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/10 14:58:25 by omiskiny          #+#    #+#             */
/*   Updated: 2026/06/10 14:58:33 by omiskiny         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	builtin_pwd(void)
{
	char	cwd[1024];

	if (getcwd(cwd, sizeof(cwd)) != NULL)
	{
		ft_putstr_fd(cwd, 1);
		write(1, "\n", 1);
	}
	else
		perror("pwd");
}

void	builtin_env(t_env *env_list)
{
	while (env_list)
	{
		if (env_list->value)
		{
			ft_putstr_fd(env_list->key, 1);
			write(1, "=", 1);
			ft_putstr_fd(env_list->value, 1);
			write(1, "\n", 1);
		}
		env_list = env_list->next;
	}
}

static void	print_echo_arg(char *arg, t_shell *sh)
{
	char	*tmp;

	if (ft_strncmp(arg, "$?", 3) == 0)
	{
		tmp = ft_itoa(sh->exit_code);
		ft_putstr_fd(tmp, 1);
		free(tmp);
	}
	else
		ft_putstr_fd(arg, 1);
}

void	builtin_echo(char **args, t_shell *sh)
{
	int	i;
	int	n_flag;

	i = 1;
	n_flag = 0;
	while (args[i] && is_n_flag(args[i]))
	{
		n_flag = 1;
		i++;
	}
	while (args[i])
	{
		print_echo_arg(args[i], sh);
		if (args[++i])
			write(1, " ", 1);
	}
	if (!n_flag)
		write(1, "\n", 1);
}

void	builtin_exit(char **args, t_shell *sh)
{
	ft_putstr_fd("exit\n", 1);
	if (!args[1])
	{
		sh->should_exit = 1;
		return ;
	}
	if (!is_numeric(args[1]))
	{
		ft_putstr_fd("minishell: exit: ", 2);
		ft_putstr_fd(args[1], 2);
		ft_putstr_fd(": numeric argument required\n", 2);
		sh->exit_code = 2;
		sh->should_exit = 1;
		return ;
	}
	if (args[2])
	{
		ft_putstr_fd("minishell: exit: too many arguments\n", 2);
		sh->exit_code = 1;
		return ;
	}
	sh->exit_code = ft_atoi(args[1]);
	sh->should_exit = 1;
}
