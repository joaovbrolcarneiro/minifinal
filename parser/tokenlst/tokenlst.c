/* ************************************************************************** */
/* */
/* :::      ::::::::   */
/* tokenlst.c                                         :+:      :+:    :+:   */
/* +:+ +:+         +:+     */
/* By: hde-barr <hde-barr@student.42.fr>          +#+  +:+       +#+        */
/* +#+#+#+#+#+   +#+           */
/* Created: 2025/03/31 21:06:35 by hde-barr          #+#    #+#             */
/* Updated: 2025/04/14 ??:??:?? by Copilot         ###   ########.fr       */
/* */
/* ************************************************************************** */


#include "minishell.h"        // Adjust includes as necessary
#include "minishell_part2.h"


 
char **get_path_list(char **env)
{
    char *path;
    char **path_list;

    path = get_env_value(env,"PATH");
    if (!path)
        return (NULL); 
    path_list = ft_split(path, ':');
    return (path_list);
}

/**
 * @brief Calculates size and allocates memory for the command list based on PATH. (MODIFIED Signature)
 *
 * @param env Environment variables.
 * @return char** Allocated char** for command list, or NULL on error.
 */
char **command_list_malloc(char **env) // MODIFIED: Removed unused 'list' parameter
{
    long long       count = 0; // Initialize count
    DIR             *dir;
    char            **path_list;
    char            **allocated_list;
    struct dirent   *entry; // To read directory entries

    path_list = get_path_list(env);
    if (!path_list)
        return (NULL); // Handle error from get_path_list

    // Count total files in all PATH directories
    int path_idx = 0;
    while (path_list[path_idx])
    {
        dir = opendir(path_list[path_idx]);
        if(dir == NULL) {
            path_idx++; // Skip directories we can't open
            continue;
        }
        while ((entry = readdir(dir)) != NULL) // Assign and check in condition
        {
             // Potentially filter out "." and ".." if not desired
             // if (ft_strcmp(entry->d_name, ".") != 0 && ft_strcmp(entry->d_name, "..") != 0)
             count++;
        }
        closedir(dir);
        path_idx++;
    }
    ft_free_strarray(path_list); // Free the path list obtained from ft_split

    // Allocate memory for the list of command names + NULL terminator
    allocated_list = malloc(sizeof(char *) * (count + 1)) ;
    if (!allocated_list)
        perror("malloc command list"); // Report allocation error

    return(allocated_list);
}

/**
 * @brief Creates a list of potential command names from PATH directories. (MODIFIED Signature)
 * Caller must free the returned array and its contents.
 *
 * @param env Environment variables.
 * @return char** Array of command name strings, or NULL on error.
 */
char **init_command_list(char **env) // MODIFIED: Removed unused 'count' parameter
{
    char            **list;
    char            **path_list;
    DIR             *dir;
    struct dirent   *d;
    long long       current_index = 0; // Use a local index

    path_list = get_path_list(env);
    if (!path_list) return (NULL);

    list = command_list_malloc(env); // Allocate memory first
    if (!list) {
        ft_free_strarray(path_list);
        return (NULL); // Return if allocation failed
    }

    // Fill the allocated list
    int path_idx = 0;
    while (path_list[path_idx])
    {
        dir = opendir(path_list[path_idx]);
        if(dir == NULL) {
             path_idx++;
             continue; // Skip directories we can't open
        }
        d = readdir(dir); // Read first entry
        while (d)
        {
             // Optionally filter "." and ".."
             // if (ft_strcmp(d->d_name, ".") != 0 && ft_strcmp(d->d_name, "..") != 0)
             // {
                 list[current_index] = ft_strdup(d->d_name);
                 if (!list[current_index]) {
                     // Handle strdup failure: free already added strings and the list
                     perror("strdup in init_command_list");
                     ft_free_strarray(list); // Custom free function handles partial list
                     list = NULL; // Mark as NULL to return
                     closedir(dir);
                     goto cleanup; // Use goto for cleanup on inner error
                 }
                 current_index++;
             // }
            d = readdir(dir); // Read next entry
        }
        closedir(dir);
        path_idx++;
    }

cleanup: // Label for cleanup
    ft_free_strarray(path_list); // Free path list regardless of success/failure
    if (list) // If list wasn't NULLed by an error
        list[current_index] = NULL; // Null-terminate the list

    return(list);
}

/**
 * @brief Searches for a command in PATH or checks if it's a valid executable path. (MODIFIED)
 * Removed unused variables 'i' and 'count'.
 *
 * @param search The command name or path to search for.
 * @param env Environment variables.
 * @return true if found/valid executable, false otherwise.
 */
bool search_list(char *search, char **env)
{
    char **list = NULL;
   
    if(is_valid_exc(search))
        return (true);

    if (!search || !env)
        return (false);

    list = init_command_list(env);
    if (!list) 
    {
         return (false);
    }
    char **current = list;
    while (*current)
    {
        if(ft_strcmp(search, *current) == 0)
        {
            ft_free_strarray(list);
            return(true);
        }
        current++;
    }

    ft_free_strarray(list);
    return(false);
}



char *get_envar(char **env, char *var)
{
	int i;

	i = -1;
	/*if (*var == '$')
		var ++;
	else
		return (NULL);*/
	while (env[++i])
	{
		if (ft_strncmp(env[i], var, ft_strnlen(env[i], '=')) == 0 \
		&& ft_strncmp(env[i], var, ft_strlen(var)) == 0)
			return (env[i] + ft_strnlen(env[i], '=') + 1);
	}
	return (NULL);
}




/*t_token	*last_node(t_token *lst)
{
	if (!lst)
		return (NULL);
	while (lst->next)
	{
		lst = lst->next;
	}
	return (lst);
}

t_token *token_lst(t_token *token)
{
	static t_token *lst;

	if (lst)
		last_node(lst)->next = token;
	else 
		lst = token;*/
/*	static t_token *lst[4096];
	static int i = 0;
	
	lst[i++] = token;
	lst[i] = NULL;*/
//	return(lst);
//}




/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   envar1.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hde-barr <hde-barr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/02 15:54:30 by hde-barr          #+#    #+#             */
/*   Updated: 2025/04/15 17:12:14 by hde-barr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
#include "../../includes/minishell.h"
#include "minishell_part2.h"

char **get_path_list(char **env)
{
	char *path;
	char **path_list;

	path = get_envar(env,"PATH");
	path_list = ft_split(path, ':');
	return (path_list);
}

char **command_list_malloc(char **list, char **env)
{
	long long i;
	DIR *dir;
	char **path_list;
	path_list = get_path_list(env);
	i = 0;
	while (*path_list)
	{
		dir = opendir(*path_list++);
		if(dir == NULL)
			continue;
		while (readdir(dir))
		{
			++i;
		}
		closedir(dir);
	}
	list = hb_malloc((sizeof(char *) * i) + 1) ;
	return(list);
}

char **init_command_list(long long count, char **env)
{
	char **list;
	char **path_list;
	DIR *dir;
	struct dirent *d;
	
	path_list = get_path_list(env);
	list = command_list_malloc(list, env);
	count = -1;
	while (*path_list)
	{
		dir = opendir(*path_list++);
		if(dir == NULL)
			continue;
		d = readdir(dir);
		while (d)
		{
			list[++count] = ft_strdup(d->d_name);
			d = readdir(dir);
		}
		closedir(dir);
	}
	list[count] = NULL;
	return(list);
}

bool search_list(char *search, char **env)
{
	char **list;
	int i = 0;
	long long count;

	if(is_valid_exc(search))
		return (true);
	if (!search || !env)
		return (false);
	list = init_command_list(count, env);
	while (*(++list))
	{
		if(ft_strncmp(search, *list, 4096) == 0)
			return(true);
	}
	return(false);
}
*/

	/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   envar2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hde-barr <hde-barr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/02 16:00:25 by hde-barr          #+#    #+#             */
/*   Updated: 2025/04/02 16:30:14 by hde-barr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"
#include "minishell_part2.h"

/*void print_env(char **env)
{
	int i;

	i = -1;
	if(!env)
	{
		ft_printf("env not setted");
		return ;
	}
	while (env[++i])
	{
		ft_printf("%s\n", env[i]);
	}
}
*/

//char *domane_expantion(char **env, char *input)
//{
//	/*if (ft_strncmp(ft_strtrim(input, " "),"env", ft_strlen(input) + 3) == 0)
//		print_env(env);*/
//	if(get_envar(env, input) /*&& *input == '$'*/)
//		return (get_envar(env, input));
//	return ("");
//}



/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   envar0.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hde-barr <hde-barr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/02 16:30:46 by hde-barr          #+#    #+#             */
/*   Updated: 2025/04/02 16:31:01 by hde-barr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
//
//#include "../../includes/minishell.h"
//#include "minishell_part2.h"
//