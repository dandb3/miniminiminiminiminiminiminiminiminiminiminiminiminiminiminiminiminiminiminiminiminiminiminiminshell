/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_utils2.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sunwsong <sunwsong@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/16 21:24:06 by jdoh              #+#    #+#             */
/*   Updated: 2023/02/23 13:07:04 by sunwsong         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execute.h"

static char	*strjoin_slash(char *s1, char *s2)
{
	char	*tmp;

	tmp = ft_strjoin(s1, "/");
	if (tmp == NULL)
		exit(MALLOC_FAILURE);
	tmp = strjoin_and_free(tmp, s2);
	return (tmp);
}

static int	find_path(char **cmd, char **path_split)
{
	struct stat	buf;
	char		*merged_path;
	int			status;

	status = FAILURE;
	while (*path_split)
	{
		merged_path = strjoin_slash(*path_split, cmd[0]);
		if (stat(merged_path, &buf) == SUCCESS && !(buf.st_mode & 0040000))
		{
			status = SUCCESS;
			free(cmd[0]);
			cmd[0] = merged_path;
			merged_path = NULL;
			if (access(cmd[0], X_OK) == SUCCESS)
				return (status);
		}
		free(merged_path);
		++path_split;
	}
	return (status);
}

static void	access_check(char *cmd, char mode)
{
	struct stat	buf;

	if (stat(cmd, &buf) == FAILURE)
	{
		write(STDERR_FILENO, SHELL, SHELL_LEN);
		if (mode == '/')
			perror_msg(cmd, 127);
		else
		{
			print_err(cmd, ": ", NULL);
			error_msg(COMMAND_NOT_FOUND, 127);
		}
	}
	else if (buf.st_mode & 0040000)
	{
		print_err(SHELL, cmd, ": ");
		error_msg(IS_A_DIR, 126);
	}
	else if (access(cmd, X_OK) == FAILURE)
	{
		write(STDERR_FILENO, SHELL, SHELL_LEN);
		perror_msg(cmd, 126);
	}
}

void	add_path_and_access_check(char **path_split, char **cmd)
{
	if (path_split == NULL)
		access_check(cmd[0], '/');
	else if (ft_strlen(cmd[0]) == 0)
	{
		print_err(SHELL, "", ": ");
		error_msg(COMMAND_NOT_FOUND, 127);
	}
	else if (ft_strchr(cmd[0], '/') != NULL)
		access_check(cmd[0], '/');
	else
	{
		if (find_path(cmd, path_split) == FAILURE)
		{
			print_err(SHELL, cmd[0], ": ");
			error_msg(COMMAND_NOT_FOUND, 127);
		}
		access_check(cmd[0], '\0');
	}
}

void	merge_wild(t_node *prev_token, t_node *cur_token, size_t len1)
{
	size_t	len2;
	size_t	idx;
	char	*str1;
	char	*str2;
	char	*merged_word;

	str1 = (char *)(prev_token->val);
	str2 = (char *)(cur_token->val);
	len2 = 0;
	if (str2)
		len2 = ft_strlen(str2);
	merged_word = (char *)ft_calloc(len1 + len2 + 1, sizeof(char));
	idx = -1;
	while (++idx < len1)
		merged_word[idx] = str1[idx];
	idx = -1;
	while (++idx < len2)
		merged_word[len1 + idx] = str2[idx];
	free(cur_token->val);
	cur_token->val = merged_word;
	del_node(prev_token, LEX);
}
