/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_check_utils1_bonus.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sunwsong <sunwsong@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/12 16:03:28 by jdoh              #+#    #+#             */
/*   Updated: 2023/02/25 13:03:43 by sunwsong         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell_bonus.h"
#include "parser_bonus.h"

static void	heredoc_sigaction_init(struct sigaction *sa)
{
	ft_memset(sa, 0, sizeof(*sa));
	sa->__sigaction_u.__sa_handler = heredoc_handler;
	sa->sa_flags = 0;
	sigaddset(&sa->sa_mask, SIGINT);
}

static char	*heredoc_readline(char *heredoc_end, char *buf)
{
	char		*total_line;
	size_t		total_len;
	ssize_t		read_len;

	total_len = 0;
	read_len = 1;
	total_line = ft_strdup("");
	if (total_line == NULL)
		exit(MALLOC_FAILURE);
	write(STDOUT_FILENO, "> ", 2);
	while ((total_len | read_len) != 0 && ft_strchr(total_line, '\n') == NULL)
	{
		read_len = read(STDIN_FILENO, buf, 1024);
		if (read_len == FAILURE)
		{
			set_heredoc_status(1);
			return ((void *)free_ret(total_line, NULL, NULL, 0));
		}
		buf[read_len] = '\0';
		total_len += (size_t) read_len;
		total_line = strjoin_and_free(total_line, buf);
	}
	if ((total_len | read_len) == 0 || !ft_strcmp(total_line, heredoc_end))
		return ((void *)free_ret(total_line, NULL, NULL, 0));
	return (total_line);
}

static void	heredoc_read(t_node **cur_token)
{
	static char	buf[1025];
	char		*cur_line;
	char		*heredoc_end;

	del_quotes((*cur_token)->val);
	heredoc_end
		= strjoin_and_free(extract_pure_word((*cur_token)->val), "\n");
	free_list((*cur_token)->val, 0, LEX);
	(*cur_token)->lex = LEX_WORD;
	(*cur_token)->val = ft_strdup("");
	if ((*cur_token)->val == NULL)
		exit(MALLOC_FAILURE);
	cur_line = heredoc_readline(heredoc_end, buf);
	while (cur_line != NULL)
	{
		(*cur_token)->val = strjoin_and_free((*cur_token)->val, cur_line);
		free(cur_line);
		cur_line = heredoc_readline(heredoc_end, buf);
	}
	free(heredoc_end);
}

void	heredoc_or_pop(t_list *stack, t_node **cur_token)
{
	struct sigaction	sa;
	struct sigaction	old_sa;

	pop_tree(stack);
	if ((*cur_token)->lex == LEX_COMPOUND \
		&& (*cur_token)->prev->lex == LEX_HERE_DOC)
	{
		heredoc_sigaction_init(&sa);
		sigaction(SIGINT, &sa, &old_sa);
		heredoc_read(cur_token);
		sigaction(SIGINT, &old_sa, NULL);
	}
	*cur_token = (*cur_token)->next;
}
