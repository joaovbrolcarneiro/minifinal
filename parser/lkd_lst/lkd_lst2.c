/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lkd_lst2.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbrol-ca <jbrol-ca@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/02 16:38:32 by hde-barr          #+#    #+#             */
/*   Updated: 2025/04/18 15:12:14 by jbrol-ca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"
#include "../../includes/minishell_part2.h"

static t_token	*advance_token_past_pipe(t_token *token)
{
	if (token && token->type == TOKEN_PIPE)
		token = token->next;
	if (token)
		token = token->next;
	return (token);
}

t_token	*remap_lst(t_token *token) // alterei - joao
{
	t_token		*first;
	t_token_vec	*token_vec; // WARNING: token_vec always NULL in this code

	first = NULL;
	token_vec = NULL;
	while (token)
	{
		handler_pipes(token_vec); // Called with NULL
		if (first == NULL)
		{
			if (token_vec) // Always false
				first = token_vec->first;
		}
		if (token_vec) // Always false
			token = token_vec->so_far;
		else
			token = NULL;
		token = advance_token_past_pipe(token); // Use helper
	}
	return (first); // Always returns NULL
}

/*t_token *remap_lst(t_token *token) ---- FUNCAO ANTIGA
{
	t_token *endof;
	t_token *first;
	t_token_vec *token_vec;

	first = hb_malloc(sizeof(t_token));
	first = NULL;
	while (token)
	{
	//	token_vec = cmd_handler_args(token);
		handler_pipes(token_vec);
		if( first == NULL)
			first = token_vec->first;
		token = token_vec->so_far;
		if(token)
			if(token_vec->so_far->type == TOKEN_PIPE)
				token = token->next;
	}
	return(first);
}
*/