/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sunwsong <sunwsong@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/26 11:41:06 by sunwsong          #+#    #+#             */
/*   Updated: 2023/02/12 13:38:49 by sunwsong         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "prompt.h"

static void	eof_exit(void)
{
	exit(EXIT_SUCCESS);
}

int	prompt(void)
{
	char	*cmd;
	char	**cmds;

	while (1)
	{
		ft_signal();
		cmd = readline("MINI$ ");
		if (!cmd)
			eof_exit();
		if (ft_strlen(cmd))
		{
			add_history(cmd);
			cmds = ft_split(cmd, ' ');
			if (!cmds)
				exit(MALLOC_FAILURE);
			if (*cmds)
				do_builtin(cmds, &g_env_list);
			free_twoptr(cmds, 0);
		}
		free(cmd);
		system("leaks minishell");
	}
	clear_history();
	return (0);
}
