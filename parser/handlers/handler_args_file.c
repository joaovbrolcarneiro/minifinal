/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handler_args_file.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbrol-ca <jbrol-ca@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/02 17:38:59 by hde-barr          #+#    #+#             */
/*   Updated: 2025/04/18 18:08:13 by jbrol-ca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"
#include "../../includes/minishell_part2.h"

t_token *rm_node_lst(t_token *token, t_token *first);


/*void join_and_split(t_token *priv, t_token *token) ---- FUNCAO ANTIGA
{
	priv->value = ft_strjoin(priv->value, "\x1F");
	priv->value = ft_strjoin(priv->value, token->value);
	priv->args = ft_split(priv->value, '\x1F');
}
*/

/*void	join_and_split(t_token *priv, t_token *token) FUNCAO ANTIGA
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
}*/

static char	*create_full_join_str(t_token *priv, t_token *token)
{
	char	*temp_join1;
	char	*temp_join2;
	char	*priv_val_or_empty;

	priv_val_or_empty = priv->value;
	if (!priv_val_or_empty)
		priv_val_or_empty = "";
	temp_join1 = ft_strjoin(priv_val_or_empty, "\x1F");
	if (!temp_join1)
		return (perror("konosubash: join_and_split: strjoin1"), NULL);
	temp_join2 = ft_strjoin(temp_join1, token->value);
	free(temp_join1); // free?
	if (!temp_join2)
		return (perror("konosubash: join_and_split: strjoin2"), NULL);
	return (temp_join2);
}

/* Joins values using separator, splits into args, handles memory */
void	join_and_split(t_token *priv, t_token *token)
{
	char	*full_str;
	char	**args_to_free;

	if (!priv || !token || !token->value)
		return ;
	args_to_free = priv->args;
	full_str = create_full_join_str(priv, token);
	if (!full_str)
		return ;
	priv->args = ft_split(full_str, '\x1F');
	free(full_str); // free?
	if (args_to_free)
		ft_free_strarray(args_to_free); // free?
	if (!priv->args)
		perror("konosubash: join_and_split: ft_split");
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


t_token	*redir_handler_file(t_token *token, t_token *first)
{
	t_token	*file_node;

	if (!token || token->coretype != REDIR || !token->next || \
		token->next->type != TOKEN_WORD)
	{
		return (token);
	}
	file_node = rm_node_lst(token->next, first);
	if (file_node)
	{
		token->file = file_node->value;
	}
	else
	{
		token->file = NULL;
		token->err = 1;
		ft_putstr_fd("konosubash: Error processing redirection filename\n", 2);
	}
	return (token);
}

t_token	*cmd_handler_args(t_token *token, t_token *first)
{
	t_token	*arg_node;

	if (!token || token->rank == RANK_S)
		return (token);
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
	return (token);
}

t_token	*handler_args_file(t_token *token, t_token *first)
{
	t_token	*current;
	t_token	*next_node;

	current = token;
	while (current)
	{
		next_node = current->next;
		redir_handler_file(current, first);
		cmd_handler_args(current, first);
		current = next_node;
	}
	return (first);
}
