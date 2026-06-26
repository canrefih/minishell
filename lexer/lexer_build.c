/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_build.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omiskiny <omiskiny@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/10 16:56:37 by omiskiny          #+#    #+#             */
/*   Updated: 2026/06/10 16:56:52 by omiskiny         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	skip_quotes(char *str, int i)
{
	char	quote;

	quote = str[i];
	i++;
	while (str[i] && str[i] != quote)
		i++;
	if (str[i] == quote)
		i++;
	return (i);
}

static int	find_word_end(char *str, int i)
{
	while (str[i] && (str[i] != ' ' && (str[i] < 9 || str[i] > 13))
		&& !is_operator(str[i]))
	{
		if (str[i] == '\'' || str[i] == '"')
			i = skip_quotes(str, i);
		else
			i++;
	}
	return (i);
}

static char	*mini_substr(const char *str, int start, int len)
{
	char	*res;
	int		i;

	res = malloc(sizeof(char) * (len + 1));
	if (!res)
		return (NULL);
	i = 0;
	while (i < len)
	{
		res[i] = str[start + i];
		i++;
	}
	res[i] = '\0';
	return (res);
}

static int	handle_operators(char *input, int i, t_token **token_list)
{
	int	start;

	start = i;
	if (input[i] == '<' && input[i + 1] == '<')
		token_add_back(token_list, create_token(mini_substr(input, start, 2),
				TOKEN_HEREDOC));
	else if (input[i] == '>' && input[i + 1] == '>')
		token_add_back(token_list, create_token(mini_substr(input, start, 2),
				TOKEN_APPEND));
	else if (input[i] == '<')
		token_add_back(token_list, create_token(mini_substr(input, start, 1),
				TOKEN_RED_IN));
	else if (input[i] == '>')
		token_add_back(token_list, create_token(mini_substr(input, start, 1),
				TOKEN_RED_OUT));
	else if (input[i] == '|')
		token_add_back(token_list, create_token(mini_substr(input, start, 1),
				TOKEN_PIPE));
	if (input[start] == '<' && input[start + 1] == '<')
		return (i + 2);
	if (input[start] == '>' && input[start + 1] == '>')
		return (i + 2);
	return (i + 1);
}

t_token	*lexer_build(char *input)
{
	t_token	*token_list;
	int		i;
	int		start;

	token_list = NULL;
	i = 0;
	while (input[i])
	{
		i = skip_spaces(input, i);
		if (!input[i])
			break ;
		if (is_operator(input[i]))
			i = handle_operators(input, i, &token_list);
		else
		{
			start = i;
			i = find_word_end(input, i);
			token_add_back(&token_list, create_token(mini_substr(input, start,
						i - start), TOKEN_WORD));
		}
	}
	return (token_list);
}
