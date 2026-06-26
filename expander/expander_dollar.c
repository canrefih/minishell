/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_dollar.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omiskiny <omiskiny@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/10 15:54:48 by omiskiny          #+#    #+#             */
/*   Updated: 2026/06/10 15:56:04 by omiskiny         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*handle_dollar_expansion(char *str, int *i,
	t_env *env_list, t_shell *sh)
{
	char	key[256];
	int		k;
	char	*value;

	(*i)++;
	if (str[*i] == '?')
	{
		(*i)++;
		return (ft_itoa(sh->exit_code));
	}
	if (!is_valid_env_char(str[*i]))
		return (mini_strdup("$"));
	k = 0;
	while (str[*i] && is_valid_env_char(str[*i]) && k < 255)
		key[k++] = str[(*i)++];
	key[k] = '\0';
	value = ft_getenv(env_list, key);
	if (!value)
		return (ft_strdup(""));
	return (mini_strdup(value));
}

static void	toggle_quote(char c, char *quote)
{
	if (*quote == 0)
		*quote = c;
	else
		*quote = 0;
}

char	*remove_quotes(char *str)
{
	char	*res;
	int		i;
	int		j;
	char	quote;

	if (!str)
		return (NULL);
	res = malloc(ft_strlen(str) + 1);
	if (!res)
		return (NULL);
	i = 0;
	j = 0;
	quote = 0;
	while (str[i])
	{
		if ((str[i] == '\'' || str[i] == '"')
			&& (quote == 0 || quote == str[i]))
			toggle_quote(str[i], &quote);
		else
			res[j++] = str[i];
		i++;
	}
	res[j] = '\0';
	free(str);
	return (res);
}
