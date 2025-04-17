/* ************************************************************************** */
/* */
/* :::      ::::::::   */
/* tokenlst.c                                         :+:      :+:    :+:   */
/* +:+ +:+         +:+     */
/* By: hde-barr <hde-barr@student.42.fr>          +#+  +:+       +#+        */
/* */ /* Updated: 2025/04/17 20:10:00 by hde-barr         ###   ########.fr       */
/* */
/* ************************************************************************** */

#include "minishell.h"
#include "minishell_part2.h" // Or relevant header for get_env_value etc.
#include <dirent.h>          // For opendir, readdir, closedir
#include <sys/stat.h>        // For stat, S_ISDIR used in is_valid_exc

/* Gets list of directories from PATH env var */
char	**get_path_list(char **env)
{
	char	*path;
	char	**path_list;

	// Use get_envar as requested, ensure it's declared if needed elsewhere
	path = get_envar(env, "PATH");
	if (!path)
		return (NULL);
	path_list = ft_split(path, ':');
	// `path` points into env, no free needed here
	return (path_list);
}

/* Helper: Counts command names in PATH dirs */
static long long	count_commands_in_path(char **env)
{
	long long		count;
	DIR				*dir;
	char			**path_list;
	struct dirent	*entry;
	int				i;

	count = 0;
	path_list = get_path_list(env);
	if (!path_list)
		return (-1);
	i = 0;
	while (path_list[i])
	{
		dir = opendir(path_list[i]);
		if (dir != NULL)
		{
			entry = readdir(dir); // Initial read
			while (entry != NULL) // Check loop condition
			{
				count++;
				entry = readdir(dir); // Read next inside loop
			}
			closedir(dir);
		}
		i++;
	}
	ft_free_strarray(path_list);
	return (count);
}

/* Allocates memory for command list based on PATH */
char	**command_list_malloc(char **env)
{
	long long	count;
	char		**allocated_list;

	allocated_list = NULL;
	count = count_commands_in_path(env);
	if (count < 0)
		return (NULL);
	allocated_list = malloc(sizeof(char *) * (count + 1));
	if (!allocated_list)
		perror("minishell: malloc command list");
	return (allocated_list);
}

/* Helper: Populates list with command names from PATH */
/* Returns 0 on success, 1 on failure */
static int	populate_command_list(char **list, char **env)
{
	char			**path_list;
	DIR				*dir;
	struct dirent	*d;
	long long		current_index;
	int				path_idx;

	current_index = 0;
	path_list = get_path_list(env);
	if (!path_list)
		return (1);
	path_idx = 0;
	while (path_list[path_idx])
	{
		dir = opendir(path_list[path_idx++]);
		if (dir != NULL)
		{
			while (1) // Separate assignment and check
			{
				d = readdir(dir);
				if (!d)
					break ;
				list[current_index] = ft_strdup(d->d_name);
				if (!list[current_index])
					return (perror("strdup"), closedir(dir), \
							ft_free_strarray(path_list), 1); // No goto
				current_index++;
			}
			closedir(dir);
		}
	}
	list[current_index] = NULL;
	ft_free_strarray(path_list);
	return (0);
}

/* Creates a list of potential command names from PATH directories */
char	**init_command_list(char **env)
{
	char	**list;
	int		populate_status;

	list = command_list_malloc(env);
	if (!list)
		return (NULL);
	populate_status = populate_command_list(list, env);
	if (populate_status != 0)
	{
		ft_free_strarray(list); // Free list if population failed
		return (NULL);
	}
	return (list);
}

/* Searches for a command in PATH or checks if it's a valid executable path */
bool	search_list(char *search, char **env)
{
	char	**list;
	char	**current;
	bool	found;

	if (!search || !env)
		return (false);
	if (is_valid_exc(search))
		return (true);
	list = init_command_list(env);
	if (!list)
		return (false);
	current = list;
	found = false;
	while (*current)
	{
		if (ft_strcmp(search, *current) == 0)
		{
			found = true;
			break ;
		}
		current++;
	}
	ft_free_strarray(list);
	return (found);
}

/* Gets environment variable value using allowed functions */
char	*get_envar(char **env, char *var)
{
	int		i;
	size_t	var_len;
	char	*eq_ptr;
	size_t	key_len;

	i = 0;
	if (!env || !var)
		return (NULL);
	var_len = ft_strlen(var);
	while (env[i])
	{
		eq_ptr = ft_strchr(env[i], '=');
		if (eq_ptr) // Check if '=' exists
		{
			key_len = eq_ptr - env[i]; // Calculate length of key part
			if (key_len == var_len && ft_strncmp(env[i], var, key_len) == 0)
				return (eq_ptr + 1); // Return pointer to value part
		}
		i++;
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