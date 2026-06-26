/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omiskiny <omiskiny@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/10 16:56:04 by omiskiny          #+#    #+#             */
/*   Updated: 2026/06/10 16:56:20 by omiskiny         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_token	*create_token(char *value, t_token_type type)
{
	t_token	*new_node;

	new_node = malloc(sizeof(t_token));
	if (!new_node)
		return (NULL);
	new_node->value = value;
	new_node->type = type;
	new_node->next = NULL;
	new_node->prev = NULL;
	new_node->heredoc_file = NULL;
	return (new_node);
}

void	token_add_back(t_token **lst, t_token *new_node)
{
	t_token	*temp;

	if (!lst || !new_node)
		return ;
	if (!*lst)
	{
		*lst = new_node;
		return ;
	}
	temp = *lst;
	while (temp->next)
		temp = temp->next;
	temp->next = new_node;
	new_node->prev = temp;
}

void	free_tokens(t_token *lexer_list)
{
	t_token	*temp;

	while (lexer_list)
	{
		temp = lexer_list->next;
		free(lexer_list->value);
		if (lexer_list->heredoc_file)
		{
			unlink(lexer_list->heredoc_file);
			free(lexer_list->heredoc_file);
		}
		free(lexer_list);
		lexer_list = temp;
	}
}

int	skip_spaces(char *str, int i)
{
	while (str[i] && (str[i] == ' ' || (str[i] >= 9 && str[i] <= 13)))
		i++;
	return (i);
}

int	is_operator(char c)
{
	return (c == '|' || c == '<' || c == '>');
}
