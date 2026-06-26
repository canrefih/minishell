/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omiskiny <omiskiny@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/10 17:14:07 by omiskiny          #+#    #+#             */
/*   Updated: 2026/06/10 17:16:00 by omiskiny         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	handle_quit_child(int sig)
{
	(void)sig;
}

void	handle_signals(int sig)
{
	if (sig == SIGINT)
	{
		g_signal = 130;
		write(1, "\n", 1);
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
	}
}

void	handle_signals_child(int sig)
{
	if (sig == SIGINT)
	{
		g_signal = 130;
		write(1, "\n", 1);
	}
}

void	init_signals(int mode)
{
	if (mode == 0)
	{
		signal(SIGQUIT, SIG_IGN);
		signal(SIGINT, handle_signals);
	}
	else if (mode == 1)
	{
		signal(SIGQUIT, handle_quit_child);
		signal(SIGINT, handle_signals_child);
	}
}
