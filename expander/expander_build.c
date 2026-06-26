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

static void	remove_empty_arg(t_cmd *cmd, int i)
{
	int	j;

	free(cmd->args[i]);
	j = i;
	while (cmd->args[j])
	{
		cmd->args[j] = cmd->args[j + 1];
		j++;
	}
}

static void	expand_args(t_expander_ctx *ctx)
{
	while (ctx->cmd->args[ctx->i])
	{
		ctx->old_val = ctx->cmd->args[ctx->i];
		ctx->cmd->args[ctx->i] = expand_word(
				ctx->old_val,
				ctx->env,
				ctx->sh);
		free(ctx->old_val);
		if (ctx->cmd->args[ctx->i]
			&& ctx->cmd->args[ctx->i][0] == '\0')
		{
			remove_empty_arg(ctx->cmd, ctx->i);
			ctx->i--;
		}
		ctx->i++;
	}
}

void	expander_build(t_cmd *cmds, t_env *env_list, t_shell *sh)
{
	t_expander_ctx	ctx;

	ctx.cmd = cmds;
	ctx.env = env_list;
	ctx.sh = sh;
	while (ctx.cmd)
	{
		if (ctx.cmd->args)
		{
			ctx.i = 0;
			expand_args(&ctx);
		}
		expand_redirects(ctx.cmd->redirects, env_list, sh);
		ctx.cmd = ctx.cmd->next;
	}
}
