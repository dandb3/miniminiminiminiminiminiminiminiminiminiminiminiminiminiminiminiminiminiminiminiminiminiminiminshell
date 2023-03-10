/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sunwsong <sunwsong@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/11 18:07:55 by jdoh              #+#    #+#             */
/*   Updated: 2023/02/14 19:25:59 by jdoh             ###   ########seoul.kr  */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

t_tree	*parser(char const *input)
{
	t_list				*token_list;
	t_tree				*abstract_syntax_tree;
	static char const	*table[9] = {T0, T1, T2, T3, T4, T5, T6, T7, T8};

	if (make_token_list(&token_list, input) == FAILURE)
		return ((void *)free_list(token_list, 0, LEX));
	if (token_list->head->next->next == NULL)
		return ((void *)free_list(token_list, 0, LEX));
	if (syntax_check(token_list, (char **)table) == FAILURE)
		return ((void *)free_list(token_list, 0, LEX));
	abstract_syntax_tree = make_ast(token_list);
	free_list(token_list, 0, LEX);
	return (abstract_syntax_tree);
}
