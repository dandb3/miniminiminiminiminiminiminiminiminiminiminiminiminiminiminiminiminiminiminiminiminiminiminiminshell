/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   doublylinkedlist2.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sunwsong <sunwsong@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/27 10:10:57 by sunwsong          #+#    #+#             */
/*   Updated: 2023/01/30 15:11:53 by sunwsong         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	insert_node(t_node *insert_point, t_node *node)
{
	insert_point->next->prev = node;
	node->next = insert_point->next;
	node->prev = insert_point;
	insert_point->next = node;
}

void	sort_list(t_list *env_list)
{
	t_node	*cur1;
	t_node	*cur2;
	t_env	*tmp;

	cur1 = env_list->head->next;
	while (cur1->next)
	{
		cur2 = cur1->next;
		while (cur2->next)
		{
			if (ft_strncmp(((t_env *)(cur1->val))->key, \
				((t_env *)(cur2->val))->key, -1) > 0)
			{
				tmp = cur1->val;
				cur1->val = cur2->val;
				cur2->val = tmp;
			}
			cur2 = cur2->next;
		}
		cur1 = cur1->next;
	}
}