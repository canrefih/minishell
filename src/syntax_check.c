/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_check.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omiskiny <omiskiny@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/10 17:07:51 by omiskiny          #+#    #+#             */
/*   Updated: 2026/06/10 17:08:12 by omiskiny         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	skip_quotes(char *input, int *i)
{
	char	quote;

	quote = input[*i];
	(*i)++;
	while (input[*i] && input[*i] != quote)
		(*i)++;
	if (!input[*i])
	{
		printf("minishell: syntax error: unclosed quote\n");
		return (1);
	}
	return (0);
}

int	check_unclosed_quotes(char *input)
{
	int	i;

	i = 0;
	while (input[i])
	{
		if (input[i] == '\'' || input[i] == '"')
		{
			if (skip_quotes(input, &i))
				return (1);
		}
		i++;
	}
	return (0);
}

static int	check_pipe_errors(t_token *tmp)
{
	if (tmp->type == TOKEN_PIPE)
	{
		if (!tmp->next || tmp->next->type == TOKEN_PIPE)
		{
			printf("minishell: syntax error near unexpected token `|'\n");
			return (1);
		}
	}
	return (0);
}

int	check_syntax_errors(t_token *tokens)
{
	t_token	*tmp;

	tmp = tokens;
	if (tmp && tmp->type == TOKEN_PIPE)
	{
		printf("minishell: syntax error near unexpected token `|'\n");
		return (1);
	}
	while (tmp)
	{
		if (check_pipe_errors(tmp))
			return (1);
		if (tmp->type == TOKEN_RED_IN || tmp->type == TOKEN_RED_OUT
			|| tmp->type == TOKEN_APPEND || tmp->type == TOKEN_HEREDOC)
		{
			if (!tmp->next || tmp->next->type != TOKEN_WORD)
			{
				printf("minishell: syntax error near unexpected token"
					" `newline'\n");
				return (1);
			}
		}
		tmp = tmp->next;
	}
	return (0);
}
