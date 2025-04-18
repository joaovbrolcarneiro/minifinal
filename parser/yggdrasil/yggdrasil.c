/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   yggdrasil.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbrol-ca <jbrol-ca@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/02 17:25:45 by hde-barr          #+#    #+#             */
/*   Updated: 2025/04/18 23:54:54 by jbrol-ca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "minishell_part2.h"

t_token	*find_left_token(t_token *t, t_token *first)
{
	t_token	*highest_node;
	t_token	*current;

	highest_node = NULL;
	if (!t || t == first)
		return (NULL);
	current = first;
	while (current && current != t)
	{
		if (!current->used)
		{
			if (!highest_node || current->rank >= highest_node->rank)
			{
				highest_node = current;
			}
		}
		current = current->next;
	}
	return (highest_node);
}

/* Finds highest priority unused token AFTER current token 't' until 'eof' */
t_token	*find_right_token(t_token *t, t_token *eof)
{
	t_token	*highest_node;
	t_token	*current;

	highest_node = NULL;
	if (!t || t == eof)
		return (NULL);
	current = t->next;
	while (current && current != eof)
	{
		if (!current->used)
		{
			if (!highest_node || current->rank >= highest_node->rank)
			{
				highest_node = current;
			}
		}
		current = current->next;
	}
	return (highest_node);
}

t_node_tree *new_yggnode(t_token *token)
{
    t_node_tree *new_node;

    new_node = hb_malloc(sizeof(t_node_tree));
	if (!new_node) 
        return (NULL);
    if (!token) 
	{
        free(new_node);
        return (NULL);
    }
    if (token->used == true)
	{
        free(new_node);
        return (NULL);
    }
    token->used = true;
    new_node->right = NULL;
    new_node->left = NULL;
    new_node->content = token->value;
    new_node->type = (t_ast_type)token->type;
    new_node->rank = token->rank;
    new_node->args = token->args;
    new_node->file = token->file;
    return (new_node);
}

/*funcao antiga
t_node_tree *new_yggnode(t_token *token)
{
    t_node_tree *new_node;

    new_node = hb_malloc(sizeof(t_node_tree));
    if (!new_node)
	{
        return (NULL);
	}
	if (!token)
        return (NULL);
	if (token->used == true)
		return (NULL);
	token->used = true;
	new_node->right = NULL;
	new_node->left = NULL;
	if (!new_node->right || !new_node->left)
		return (NULL);
	new_node->content = token->value;
	new_node->type = (t_ast_type)token->type;
	new_node->rank = token->rank;
	new_node->args = token->args;
	new_node->file = token->file;
    return (new_node);
}*/

t_node_tree *make_yggdrasil(t_token *t, t_token *f, t_token *e, t_node_tree *y)
{
    if (!y)
        y = new_yggnode(t);

    // Handle redirection nodes differently
    if (t && t->coretype == REDIR) {
        // Left child: command + arguments
        y->left = build_command_subtree(f, t);
        
        // Right child: redirection target file
        if (t->next && !t->next->used) {
            y->right = new_yggnode(t->next);
            t->next->used = true;
        }
    } else {
        // Original logic for other node types
        y->left = new_yggnode(find_left_token(t, f));
        y->right = new_yggnode(find_right_token(t, e));
    }

    if (y->left && y->left->content)
        make_yggdrasil(find_left_token(t, f), f, t, y->left);
    if (y->right && y->right->content)
        make_yggdrasil(find_right_token(t, e), f, e, y->right);

    return (y);
}

// Helper function to build command+arguments subtree
t_node_tree *build_command_subtree(t_token *first, t_token *redir)
{
    t_node_tree *root = NULL;
    t_node_tree *current = NULL;
    t_token *tmp = first;

    while (tmp && tmp != redir) {
        if (!tmp->used && tmp->coretype != REDIR) {
            t_node_tree *new = new_yggnode(tmp);
            if (!root) {
                root = new;
                current = root;
            } else {
                current->right = new;
                current = current->right;
            }
            tmp->used = true;
        }
        tmp = tmp->next;
    }
    return (root);
}

/*t_node_tree	*make_yggdrasil(t_token *t, t_token *f, t_token *e, t_node_tree *y)////////yggdrasil
{ FUNCAO ANTIGA
	//validation_tree(t, f);
	//take_args();
	if(!y)
		y = new_yggnode(t);
	y->left = new_yggnode(find_left_token(t, f));
	y->right = new_yggnode(find_right_token(t, e));
	if (y->left)
		make_yggdrasil(find_left_token(t, f), f, t, y->left);
	if (y->right)
		make_yggdrasil(find_right_token(t, e), f, e, y->right);
	return (y);
}
*/