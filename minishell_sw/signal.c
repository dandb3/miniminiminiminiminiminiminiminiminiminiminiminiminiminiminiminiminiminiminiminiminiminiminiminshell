/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sunwsong <sunwsong@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/31 14:28:13 by sunwsong          #+#    #+#             */
/*   Updated: 2023/02/12 13:23:05 by sunwsong         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <signal.h>
#include "prompt.h"

void	handler(int sig)
{
	ft_printf("\n");
	rl_on_new_line();
	rl_replace_line("", 1);
	rl_redisplay();
	set_exitcode(1);
	(void) sig;
}

int	ft_signal(void)
{
	struct sigaction	act_int;
	struct sigaction	act_quit;

	act_int.sa_flags = SA_RESTART;
	act_int.__sigaction_u.__sa_handler = handler;
	act_quit.sa_flags = SA_RESTART;
	act_quit.__sigaction_u.__sa_handler = SIG_IGN;
	if (sigaction(SIGINT, &act_int, NULL) | sigaction(SIGQUIT, &act_quit, NULL))
	{
		perror("");
		exit(EXIT_FAILURE);
	}
	return (SUCCESS);
}
