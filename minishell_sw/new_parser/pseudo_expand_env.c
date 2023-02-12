/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pseudo_expand_env.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sunwsong <sunwsong@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/03 18:40:44 by jdoh              #+#    #+#             */
/*   Updated: 2023/02/12 19:55:40 by sunwsong         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

static void	env_val(t_node *cur_token)
{
	size_t	idx;
	char	*tmp1;
	char	*tmp2;

	idx = get_envlen(cur_token->next->val);
	if (idx == 0)
	{
		cur_token->lex = LEX_WORD;
		return ;
	}
	if (idx == ft_strlen(cur_token->next->val))
	{
		free(cur_token->val);
		cur_token->val = cur_token->next->val;
		cur_token->next->val = NULL;
		del_node(cur_token->next, LEX);
	}
	tmp1 = ft_substr(cur_token->next->val, 0, idx);
	tmp2 = ft_substr(cur_token->next->val, idx, -1);
	if (tmp1 == NULL || tmp2 == NULL)
		exit(MALLOC_FAILURE);
	free(cur_token->next->val);
	free(cur_token->val);
	cur_token->val = tmp1;
	cur_token->next->val = tmp2;
}

static void	substitute_env(t_node *cur_token, t_flag flag)
{
	if (cur_token->next->next == NULL)
	{
		cur_token->lex = LEX_WORD;
		return ;
	}
	if (flag == UN_QUOTED && (cur_token->next->lex == LEX_SINGLE_QUOTE
			|| cur_token->next->lex == LEX_DOUBLE_QUOTE))
		del_node(cur_token, LEX);
	else if (cur_token->next->lex == LEX_WORD)
		env_val(cur_token);
	else
		cur_token->lex = LEX_WORD;
}

int	pseudo_expand_env(t_list *token_list)
{
	t_node	*cur_token;
	t_flag	flag;

	flag = UN_QUOTED;
	cur_token = token_list->head->next;
	while (cur_token->next != NULL)
	{
		if (flag == UN_QUOTED && cur_token->lex == LEX_SINGLE_QUOTE)
			flag = SINGLE_QUOTED;
		else if (flag == UN_QUOTED && cur_token->lex == LEX_DOUBLE_QUOTE)
			flag = DOUBLE_QUOTED;
		else if ((flag == SINGLE_QUOTED && cur_token->lex == LEX_SINGLE_QUOTE)
			|| (flag == DOUBLE_QUOTED && cur_token->lex == LEX_DOUBLE_QUOTE))
			flag = UN_QUOTED;
		else if ((flag == UN_QUOTED || flag == DOUBLE_QUOTED)
			&& cur_token->lex == LEX_ENV)
			substitute_env(cur_token, flag);
		else if (flag == DOUBLE_QUOTED || flag == SINGLE_QUOTED)
			cur_token->lex = LEX_WORD;
		cur_token = cur_token->next;
	}
	if (flag != UN_QUOTED)
		return (FAILURE);
	return (SUCCESS);
}
