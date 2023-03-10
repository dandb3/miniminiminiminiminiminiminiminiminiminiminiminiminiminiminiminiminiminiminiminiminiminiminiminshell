/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_bonus.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sunwsong <sunwsong@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/23 19:43:41 by jdoh              #+#    #+#             */
/*   Updated: 2023/02/25 13:03:43 by sunwsong         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_BONUS_H
# define PARSER_BONUS_H

# include "../minishell_bonus.h"
# define T0 "\1\0\0\0\1\0\1\1\1\1\0"
# define T1 "\0\2\3\0\0\4\0\0\0\0\4"
# define T2 "\5\0\0\0\5\0\5\5\5\5\0"
# define T3 "\0\7\7\6\0\7\0\0\0\0\7"
# define T4 "\0\11\11\11\0\11\10\10\10\10\11"
# define T5 "\13\0\0\0\12\0\13\13\13\13\0"
# define T6 "\14\15\15\15\0\15\14\14\14\14\15"
# define T7 "\16\0\0\0\0\0\17\17\17\17\0"
# define T8 "\0\0\0\0\0\0\20\21\22\23\0"

typedef enum e_flag
{
	UN_QUOTED,
	SINGLE_QUOTED,
	DOUBLE_QUOTED
}		t_flag;

typedef enum e_table
{
	SYNTAX_ERROR,
	E0_E2_E1,
	E1_OR_E0,
	E1_AND_E0,
	E1_EPSILON,
	E2_E5_E3,
	E3_PIPE_E2,
	E3_EPSILON,
	E4_E8_E4,
	E4_EPSILON,
	E5_PARENTHESES,
	E5_E7_E6,
	E6_E7_E6,
	E6_EPSILON,
	E7_COMP,
	E7_E8,
	E8_REDIRECT_IN,
	E8_REDIRECT_OUT,
	E8_HERE_DOC,
	E8_REDIRECT_APPEND
}	t_table;

/*---------------------------------tokenize---------------------------------*/
t_node		*get_pseudo_token(char const **str);
int			pseudo_expand_env(t_list *token_list);
void		del_quotes(t_list *token_list);
void		del_whitespace(t_list *token_list);
void		group_compound(t_list *token_list);

/*----------------------------------stack-----------------------------------*/
void		pop(t_list *stack);
void		pop_tree(t_list *stack);
void		push(t_list *stack, t_tree *tree_node);
int			stack_empty(t_list *stack);
t_node		*top(t_list *stack);

/*-----------------------------------tree-----------------------------------*/
t_tree		*make_tree(t_symbol symbol);
void		free_tree(t_tree *root);

/*--------------------------------syntax_tree-------------------------------*/
void		syntax_init(t_node **cur_token, t_list **stack, t_list *token_list);
int			syntax_check(t_list *token_list, char **table);
void		production(t_list *stack, t_table table_result);
int			error_manage(t_node *cur_token, t_list *stack);

/*------------------------------------ast-----------------------------------*/
t_tree		*make_ast(t_list *token_list);
t_tree		*make_empty_tree(t_node **cur_token);
t_tree		*make_expression(t_node **cur_token);

/*-----------------------------------utils----------------------------------*/
int			is_compound_redirect(t_lex lex);
int			is_redirect(t_lex lex);
t_symbol	lex_to_symbol(t_lex lex);
int			is_terminal(t_symbol symbol);
int			table_idx(t_lex lex);
char		*extract_pure_word(t_list *compound_list);
void		heredoc_or_pop(t_list *stack, t_node **cur_token);

/*---------------------------------here_doc---------------------------------*/
void		set_heredoc_status(int status);
int			get_heredoc_status(void);
void		heredoc_handler(int sig);

#endif
