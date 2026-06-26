/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omiskiny <omiskiny@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/10 17:08:38 by omiskiny          #+#    #+#             */
/*   Updated: 2026/06/10 17:12:38 by omiskiny         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	g_signal = 0;

static int	cleanup_and_exit(char *input, t_token *tokens,
	int code, t_shell *sh)
{
	if (tokens)
		free_tokens(tokens);
	if (input)
		free(input);
	sh->exit_code = code;
	return (1);
}

static int	process_input(char *input, t_env *env, t_shell *sh)
{
	t_token	*tokens;
	t_cmd	*cmds;

	if (check_unclosed_quotes(input))
		return (cleanup_and_exit(input, NULL, 2, sh));
	tokens = lexer_build(input);
	if (check_syntax_errors(tokens))
		return (cleanup_and_exit(input, tokens, 2, sh));
	cmds = parser_build(tokens);
	if (!cmds)
		return (cleanup_and_exit(input, tokens, 2, sh));
	expander_build(cmds, env, sh);
	init_signals(1);
	sh->exit_code = executor_build(cmds, &env, sh);
	init_signals(0);
	free_cmds(cmds);
	free_tokens(tokens);
	free(input);
	return (0);
}

static int	handle_input_checks(char **input)
{
	if (!*input)
	{
		write(1, "exit\n", 5);
		return (-1);
	}
	if (**input)
		add_history(*input);
	return (0);
}

static void	update_exit_code_signal(t_shell *sh)
{
	if (g_signal == 130)
	{
		sh->exit_code = 130;
		g_signal = 0;
	}
}

int	main(int ac, char **av, char **env)
{
	char	*input;
	t_env	*env_list;
	t_shell	sh;

	(void)ac;
	(void)av;
	sh.exit_code = 0;
	sh.should_exit = 0;
	env_list = init_env_list(env);
	init_signals(0);
	while (1)
	{
		update_exit_code_signal(&sh);
		input = readline("minishell$ ");
		if (!input)
			break ;
		if (handle_input_checks(&input) == 0 && *input)
			process_input(input, env_list, &sh);
		else
			free(input);
		if (sh.should_exit)
			break ;
	}
	free_env_list(env_list);
	return (sh.exit_code);
}
