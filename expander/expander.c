/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omiskiny <omiskiny@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/10 15:54:07 by omiskiny          #+#    #+#             */
/*   Updated: 2026/06/10 15:54:29 by omiskiny         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	expand_dollar(t_expand_ctx *ctx)
{
	char	*expansion;

	expansion = handle_dollar_expansion(ctx->word,
			&ctx->i, ctx->env_list, ctx->sh);
	if (expansion)
	{
		ctx->res = join_and_free(ctx->res, expansion);
		free(expansion);
	}
}

static int	process_expand_char(t_expand_ctx *ctx)
{
	if (ctx->word[ctx->i] == '\'' && !ctx->dq)
	{
		ctx->sq = !ctx->sq;
		ctx->i++;
		return (1);
	}
	if (ctx->word[ctx->i] == '"' && !ctx->sq)
	{
		ctx->dq = !ctx->dq;
		ctx->i++;
		return (1);
	}
	if (ctx->word[ctx->i] == '$' && !ctx->sq)
	{
		expand_dollar(ctx);
		return (1);
	}
	return (0);
}

char	*expand_word(char *word, t_env *env_list, t_shell *sh)
{
	t_expand_ctx	ctx;

	ctx.word = word;
	ctx.res = mini_strdup("");
	ctx.env_list = env_list;
	ctx.sh = sh;
	ctx.i = 0;
	ctx.sq = 0;
	ctx.dq = 0;
	while (ctx.word[ctx.i])
	{
		if (process_expand_char(&ctx))
			continue ;
		ctx.res = add_char_to_str(ctx.res, ctx.word[ctx.i]);
		ctx.i++;
	}
	return (ctx.res);
}

void	expand_redirects(t_token *redir, t_env *env_list, t_shell *sh)
{
	char	*old_val;

	while (redir)
	{
		if (redir->type == TOKEN_WORD)
		{
			old_val = redir->value;
			redir->value = expand_word(old_val, env_list, sh);
			free(old_val);
		}
		redir = redir->next;
	}
}
