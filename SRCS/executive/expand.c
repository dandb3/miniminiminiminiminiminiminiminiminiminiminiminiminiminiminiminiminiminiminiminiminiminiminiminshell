/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sunwsong <sunwsong@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/14 20:46:23 by sunwsong          #+#    #+#             */
/*   Updated: 2023/02/21 21:44:01 by jdoh             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execute.h"

static size_t	expand_env(t_node *env_token)
{
	size_t	len;
	char	*val;

	val = find_env_val(env_token->val);
	free(env_token->val);
	env_token->val = val;
	if (val == NULL)
		return (0);
	len = ft_strlen(val);
	return (len);
}

static void	replace_asterisk(t_node *cur_token, size_t total_len, size_t wlen)
{
	size_t	idx;
	char	*val;

	val = (char *)(cur_token->val);
	idx = total_len - wlen;
	while (idx < total_len)
	{
		if (val[idx] == '*')
			val[idx] = 0;
		++idx;
	}
}

static size_t	join_tokens(t_node *cur_token, int *is_wild)
{
	size_t	total_len;
	size_t	cur_len;

	total_len = 0;
	while (cur_token->next != NULL)
	{
		cur_len = ft_strlen(cur_token->val);
		if (cur_token->lex == LEX_ENV)
			cur_len = expand_env(cur_token);
		else if (cur_token->lex == LEX_WILD)
			*is_wild = TRUE;
		if (cur_token->prev->lex != -1)
			merge_wild(cur_token->prev, cur_token, total_len);
		total_len += cur_len;
		if (cur_token->lex == LEX_WILD)
			replace_asterisk(cur_token, total_len, cur_len);
			cur_token = cur_token->next;
	}
	return (total_len);
}

t_list	*expand_char(t_list *compound_list)
{
	t_list	*res;
	size_t	len;
	int		is_wild;

	is_wild = FALSE;
	del_quotes(compound_list);
	len = join_tokens(compound_list->head->next, &is_wild);
	if (is_wild == TRUE)
		res = wildcard(compound_list->head->next->val, len);
	else
	{
		res = make_list(NAME);
		if (compound_list->head->next->val)
			push_node(make_node(compound_list->head->next->val, -1), res);
		else
			push_node(make_null_value_node(-1), res);
		compound_list->head->next->val = NULL;
	}
	return (res);
}
