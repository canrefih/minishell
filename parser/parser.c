/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omiskiny <omiskiny@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/10 16:39:23 by omiskiny          #+#    #+#             */
/*   Updated: 2026/06/10 16:40:38 by omiskiny         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	handle_parser_redirect(t_cmd *current_cmd, t_token *tokens)
{
	t_token	*redir_token;
	t_token	*file_token;

	if (!tokens->next || tokens->next->type != TOKEN_WORD)
	{
		ft_putstr_fd("minishell: syntax error near unexpected token\n", 2);
		return (0);
	}
	redir_token = create_token(ft_strdup(tokens->value), tokens->type);
	file_token = create_token(ft_strdup(tokens->next->value), TOKEN_WORD);
	token_add_back(&(current_cmd->redirects), redir_token);
	token_add_back(&(current_cmd->redirects), file_token);
	return (1);
}

static int	parse_token_nodes(t_cmd **cmd_list, t_cmd **curr, t_token **tmp)
{
	if ((*tmp)->type == TOKEN_PIPE)
	{
		*curr = create_cmd_node();
		if (!*curr)
			return (0);
		cmd_add_back(cmd_list, *curr);
		*tmp = (*tmp)->next;
		return (1);
	}
	if ((*tmp)->type == TOKEN_WORD)
	{
		(*curr)->args = add_arg_to_matrix((*curr)->args, (*tmp)->value);
		*tmp = (*tmp)->next;
		return (1);
	}
	return (0);
}

static int	handle_syntax_error(char *msg, t_cmd *list)
{
	ft_putstr_fd(msg, 2);
	if (list)
		free_cmds(list);
	return (1);
}

t_cmd	*parser_build(t_token *tokens)
{
	t_cmd	*list;
	t_cmd	*curr;
	t_token	*tmp;

	if (tokens && tokens->type == TOKEN_PIPE)
		return (ft_putstr_fd("minishell: syntax error near `|'\n", 2), NULL);
	list = create_cmd_node();
	curr = list;
	tmp = tokens;
	while (tmp)
	{
		if (parse_token_nodes(&list, &curr, &tmp))
			continue ;
		if (!tmp->next)
		{
			handle_syntax_error("minishell: syntax error "
				"`newline'\n", list);
			return (NULL);
		}
		handle_parser_redirect(curr, tmp);
		tmp = tmp->next->next;
	}
	return (list);
}
