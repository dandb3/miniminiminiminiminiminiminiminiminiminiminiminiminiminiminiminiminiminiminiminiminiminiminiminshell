/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sunwsong <sunwsong@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/24 10:33:52 by sunwsong          #+#    #+#             */
/*   Updated: 2023/02/05 16:22:41 by sunwsong         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h> // 지우세용
# include <unistd.h>
# include <stdlib.h>
# include <fcntl.h>
# include <signal.h>
# include <limits.h>
# include "libft/libft.h"

# define MALLOC_FAILURE 2

enum	e_bool
{
	FALSE = 0,
	TRUE
};

typedef enum e_lex
{
	LEX_SINGLE_QUOTE,
	LEX_DOUBLE_QUOTE,
	LEX_REDIRECT_IN,
	LEX_REDIRECT_OUT,
	LEX_HERE_DOC,
	LEX_REDIRECT_APP,
	LEX_PIPE,
	LEX_ENV,
	LEX_AND,
	LEX_OR,
	LEX_WILD,
	LEX_WORD,
	LEX_PARENTHESIS_OPEN,
	LEX_PARENTHESIS_CLOSE,
	LEX_WHITE
}			t_lex;

enum e_sbool
{
	FAILURE = -1,
	SUCCESS
};

typedef enum e_type
{
	ENV = 0,
	LEX = 1,
	NAME = 0
}	t_type;

typedef struct s_node
{
	void			*val;
	struct s_node	*next;
	struct s_node	*prev;
	t_lex			lex;
}	t_node;

typedef struct s_env
{
	char			*key;
	char			*val;
}	t_env;

typedef struct s_list
{
	struct s_node	*head;
	struct s_node	*tail;
	int				size;
	t_type			type;
}	t_list;

/*--------------------------------  main  ---------------------------------*/
int			ft_signal(void);
int			ft_terminal(void);

/*-------------------------------- builtin --------------------------------*/
int			do_builtin(char **cmds, t_list **env_list);
int			prompt(t_list *env_list);

/*--------------------------- doubly linked list --------------------------*/
t_list		*make_list(t_type type);
void		push_node(t_node *node, t_list *list);
void		insert_node(t_node *insert_point, t_node *node);
long long	free_nodes(t_list *list, long long ret, t_type type);
long long	free_list(t_list *list, long long ret, t_type type);
void		sort_list(t_list *env_list);

/*------------------------------- node list -------------------------------*/
t_node		*make_node(void *val, t_lex lex);
void		del_node(t_node *node, t_type type);
void		free_node(t_node *node, t_type type);

/*--------------------------------- utils ---------------------------------*/
long long	free_ret(void *obj1, void *obj2, void *obj3, long long ret);
long long	free_twoptr(char **ptr, long long ret);

/*--------------------------------- utils ---------------------------------*/
size_t		envlen(const char *str);
t_env		*make_env(const char *str, int to_check);
int			push_environ(t_env *env, t_node *cur);
int			print_envlist(t_list *list, const char *pre);
t_list		*make_envlist(char **envp);
char		**env_to_char(t_list *env_list);

/*lexer*/
int			make_token_list(t_list **token_list, t_list *env_list, \
	char const *str);

#endif