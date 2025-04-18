/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   merge_to_token_exception.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbrol-ca <jbrol-ca@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/02 19:17:54 by hde-barr          #+#    #+#             */
/*   Updated: 2025/04/18 15:50:09 by jbrol-ca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"
#include "../../includes/minishell_part2.h"


bool	merge_to_token_exception(t_token *token, t_token *list_head) // alterei - JOAO
{
	t_token	*rm_node;
	char	*original_value;
	char	*new_value;

	if (!token || !token->next || !list_head || token->merge_next == false)
		return (false);
	rm_node = rm_node_lst(token->next, list_head); // Assumes rm_node_lst declared
	if (!rm_node || !rm_node->value)
	{
		return (false);
	}
	original_value = token->value;
	new_value = ft_strjoin(original_value, rm_node->value);
	if (!new_value)
	{
		perror("konosubash: merge: ft_strjoin failed");
		return (false);
	}
	// free(original_value); free?
	token->value = new_value; 
	token->merge_next = false; // Prevent re-merging attempt
	// partner_gc_free(rm_node->value); free?
	// partner_gc_free(rm_node); free?
	return (true);
}

/*bool merge_to_token_exception(t_token *token) - funcao antiga
{
	t_token *first;
	t_token *rm_node;
	first = token;
	if(token->merge_next == true)
	{
		rm_node = rm_node_lst(token->next, first);
		token->value = ft_strjoin(token->value, rm_node->value);
	}
	return (true);
}*/