/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handler_args_file.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbrol-ca <jbrol-ca@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/02 17:38:59 by hde-barr          #+#    #+#             */
/*   Updated: 2025/04/17 16:52:29 by jbrol-ca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "minishell_part2.h"

t_token *rm_node_lst(t_token *token, t_token *first);


/*void join_and_split(t_token *priv, t_token *token) ---- FUNCAO ANTIGA
{
	priv->value = ft_strjoin(priv->value, "\x1F");
	priv->value = ft_strjoin(priv->value, token->value);
	priv->args = ft_split(priv->value, '\x1F');
}
*/

void	join_and_split(t_token *priv, t_token *token) // ALTEREI - JOAO
{
	char	*temp_join1;
	char	*temp_join2;
	char	**args_to_free;

	if (!priv || !token || !token->value)
		return ;
	args_to_free = priv->args;
	if (!priv->value)
		priv->value = ft_strdup("");
	if (!priv->value)
		return ;
	temp_join1 = ft_strjoin(priv->value, "\x1F");
	if (!temp_join1)
		return (perror("minishell: join_and_split: strjoin1"));
	temp_join2 = ft_strjoin(temp_join1, token->value);
	free(temp_join1);
	if (!temp_join2)
		return (perror("minishell: join_and_split: strjoin2"));
	priv->args = ft_split(temp_join2, '\x1F');
	free(temp_join2);
	if (args_to_free)
		ft_free_strarray(args_to_free);
	if (!priv->args)
		perror("minishell: join_and_split: ft_split");
}

/* FUNCAO ANTIGA
t_token *redir_handler_file(t_token *token ,t_token *first)
{
	if (token->coretype == REDIR && token->next)
		if (token->next->rank != RANK_S)
		{
			ft_printf(YLW"%s\n",token->value);
			ft_printf(PNK"%s\n",token->next->value);
			//token->next->used = true;
			token->file = rm_node_lst(token->next, first)->value;
		}
	return (token);
}
*/


t_token	*redir_handler_file(t_token *token, t_token *first) // alterei  - joao
{
	t_token	*file_node;

	if (!token || token->coretype != REDIR || !token->next || \
		token->next->rank == RANK_S)
		return (token);
	file_node = rm_node_lst(token->next, first);
	if (file_node)
	{
		token->file = file_node->value;
	}
	else
	{
		token->file = NULL;
	}
	return (token);
}

t_token	*cmd_handler_args(t_token *token, t_token *first) // ALTEREI - JOAO
{
	t_token	*redir;
	t_token	*arg_node;

	redir = NULL;
	while (token && token->rank != RANK_S)
	{
		while (token->next && token->next->rank != RANK_S)
		{
			if (token->next->coretype == REDIR)
				break ;
			else
			{
				arg_node = rm_node_lst(token->next, first);
				if (!arg_node)
					break ;
				join_and_split(token, arg_node);
			}
		}
		if (redir)
			add_node_lst(token, redir);
		if (redir)
			redir = NULL;
		token = token->next;
	}
	return (first);
}

t_token	*handler_args_file(t_token *token, t_token *first)
{
	t_token	*current;

	current = token;
	while (current)
	{
		redir_handler_file(current, first);
		cmd_handler_args(current, first);
		current = current->next;
	}
	return (first);
}
