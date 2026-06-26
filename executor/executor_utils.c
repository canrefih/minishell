/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omiskiny <omiskiny@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/10 15:15:34 by omiskiny          #+#    #+#             */
/*   Updated: 2026/06/10 15:15:53 by omiskiny         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_split(char **arr)
{
	int	i;

	i = 0;
	if (!arr)
		return ;
	while (arr[i])
	{
		free(arr[i]);
		i++;
	}
	free(arr);
}

static char	*try_path(char *dir, char *cmd)
{
	char		*tmp;
	char		*full;
	struct stat	st;

	tmp = ft_strjoin(dir, "/");
	full = ft_strjoin(tmp, cmd);
	free(tmp);
	if (full && access(full, X_OK) == 0)
	{
		if (stat(full, &st) == 0 && !S_ISDIR(st.st_mode))
			return (full);
	}
	free(full);
	return (NULL);
}

static char	*check_path_loop(char **dirs, char *cmd)
{
	char	*path;
	int		i;

	if (!cmd || !*cmd || !dirs)
		return (free_split(dirs), NULL);
	i = 0;
	while (dirs[i])
	{
		path = try_path(dirs[i], cmd);
		if (path)
		{
			free_split(dirs);
			return (path);
		}
		i++;
	}
	free_split(dirs);
	return (NULL);
}

char	*find_cmd(char *cmd, char **envp)
{
	char	**dirs;
	int		i;

	if (!cmd || cmd[0] == '\0' || !envp || !*envp)
		return (NULL);
	if (ft_strchr(cmd, '/') && access(cmd, X_OK) == 0)
		return (ft_strdup(cmd));
	i = 0;
	while (envp[i] && ft_strncmp(envp[i], "PATH=", 5) != 0)
		i++;
	if (!envp[i])
		return (NULL);
	dirs = ft_split(envp[i] + 5, ':');
	return (check_path_loop(dirs, cmd));
}

char	**convert_env_list_to_matrix(t_env *env_list)
{
	t_env	*tmp;
	int		count;
	char	**matrix;
	int		i;
	char	*tmp_join;

	tmp = env_list;
	count = 0;
	while (tmp && ++count)
		tmp = tmp->next;
	matrix = malloc(sizeof(char *) * (count + 1));
	if (!matrix)
		return (NULL);
	tmp = env_list;
	i = 0;
	while (tmp)
	{
		tmp_join = ft_strjoin(tmp->key, "=");
		matrix[i] = ft_strjoin(tmp_join, tmp->value);
		free(tmp_join);
		i++;
		tmp = tmp->next;
	}
	matrix[i] = NULL;
	return (matrix);
}
