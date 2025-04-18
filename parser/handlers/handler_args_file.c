/* ************************************************************************** */
/* */
/* :::      ::::::::   */
/* handler_args_file.c                                :+:      :+:    :+:   */
/* +:+ +:+         +:+     */
/* By: hde-barr <hde-barr@student.42.fr>          +#+  +:+       +#+        */
/* */ /* Updated: 2025/04/18 18:30:00 by hde-barr         ###   ########.fr       */
/* */
/* ************************************************************************** */

#include "../../includes/minishell.h"
#include "../../includes/minishell_part2.h" // For t_token definition etc.

// Forward declaration needed for cmd_handler_args calling join_and_split
void		join_and_split(t_token *priv, t_token *token);
// Forward declaration needed for handler_args_file calling handlers
t_token		*redir_handler_file(t_token *token, t_token *first);
t_token		*cmd_handler_args(t_token *token, t_token *first);
// Assumes rm_node_lst and other necessary functions are declared elsewhere

/* Helper: Reallocates args array, adding new_arg. Frees old array ptr. */
static char	**realloc_args_array(char **old_args, int old_argc, \
									const char *new_arg)
{
	char	**new_args;
	int		i;

	new_args = malloc(sizeof(char *) * (old_argc + 2));
	if (!new_args)
		return (perror("minishell: malloc args array"), NULL);
	i = 0;
	while (i < old_argc)
	{
		new_args[i] = old_args[i];
		i++;
	}
	new_args[i] = ft_strdup(new_arg);
	if (!new_args[i])
	{
		perror("minishell: strdup new arg");
		free(new_args);
		// Old pointers in new_args[0..i-1] are aliases, not freed here.
		// Caller needs to handle cleanup if necessary.
		return (NULL);
	}
	new_args[i + 1] = NULL;
	if (old_args)
		free(old_args); // Free only the array structure, not content
	return (new_args);
}

/* Helper: Creates the initial args array {"cmd", "arg1", NULL} */
static int	create_first_arg(t_token *priv, t_token *arg_token)
{
	char	**new_args;

	if (!priv || !priv->value || !arg_token || !arg_token->value)
		return (1);
	new_args = malloc(sizeof(char *) * 3);
	if (!new_args)
		return (perror("minishell: malloc first arg"), 1);
	// IMPORTANT: args[0] MUST point to the original command name string
	// for consistency and correct execution later. Do NOT strdup here.
	new_args[0] = priv->value;
	new_args[1] = ft_strdup(arg_token->value); // First arg VALUE is duplicated
	if (!new_args[1])
		return (perror("minishell: strdup first arg"), free(new_args), 1);
	new_args[2] = NULL;
	// If priv->args existed (shouldn't for first arg), free it
	if (priv->args)
		free(priv->args); // Only free the array pointer itself
	priv->args = new_args;
	return (0);
}

/* Replaces old string join/split logic with array building logic */
/* Adds the value of arg_token to the priv->args array */
void	join_and_split(t_token *priv, t_token *arg_token)
{
	int		argc;
	char	**temp_args;

	if (!priv || !arg_token || !arg_token->value)
		return ;
	if (priv->args == NULL) // First argument being added
	{
		if (create_first_arg(priv, arg_token) != 0)
			priv->args = NULL; // Ensure args is NULL on error
	}
	else // Subsequent arguments
	{
		argc = 0;
		while (priv->args[argc] != NULL)
			argc++;
		temp_args = realloc_args_array(priv->args, argc, arg_token->value);
		// realloc_args_array frees the old priv->args pointer on success
		if (!temp_args)
			priv->args = NULL; // Ensure args is NULL on error
		else
			priv->args = temp_args;
	}
	// Partner's GC handles freeing the original arg_token->value string later
}

/* Associates filename (must be a WORD) with redirection token */
t_token	*redir_handler_file(t_token *token, t_token *first)
{
	t_token	*file_node;

	if (!token || token->coretype != REDIR || !token->next || \
		token->next->type != TOKEN_WORD)
		return (token);
	file_node = rm_node_lst(token->next, first);
	if (file_node)
	{
		token->file = file_node->value;
		// Partner's GC handles freeing file_node struct
	}
	else
	{
		token->file = NULL;
		token->err = 1;
		ft_putstr_fd("konosubash: Error processing redirection filename\n", 2);
	}
	return (token);
}

/* Processes arguments for a command token */
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
			join_and_split(token, arg_node); // Calls corrected logic
			if (!token->args) // Check if join_and_split failed
				break; // Stop processing args if malloc failed
			// Partner's GC must handle freeing the unlinked arg_node struct
		}
	}
	return (token);
}

/* Top-level handler for arguments and file associations */
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