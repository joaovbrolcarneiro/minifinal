/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_input.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbrol-ca <jbrol-ca@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/02 16:53:41 by hde-barr          #+#    #+#             */
/*   Updated: 2025/04/17 19:16:49 by jbrol-ca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"
#include "../../includes/minishell_part2.h"

bool proximity_exception(char *input, int i)
{
	if((!ischarset("|<>", input[i]) && input[i] != ' ') && \
    input[i] && !ischarset("|<>", *input ))
		return (true);
	return (false);
}

//t_token *split_input(char *input, int i)
//{
//   t_token *lst;
//    t_token *next;
//	t_token *first;
//
//    lst = hb_malloc(sizeof(t_token));
//	first = lst;
//    while(*input)
//    {
//		i = 1;
//		while (*input && *input == ' ')
//			input++;
//		if(!*input)
//			return (lst->next = NULL, lst->value = "", first);
//		if(ischarset("\"'", *input))
//			while(input[i] != *input && input[i] /*|| input[i + 1] != '\0'*/)//????
//				i++;
//		else
//			i = ft_strsetlen(input, "\"' |<>");
//		if(input[i] == *input)
//			i++;
//		if(ischarset("<>", *input))
//			if (*(input + 1) == *input)
//				i++;
//		//i++;
//		lst->merge_next = proximity_exception(input, i);
//		lst->value = ft_substr(input, 0, i);
//		lst->next = hb_malloc(sizeof(t_token));
//		input += i;
//		while (*input && *input == ' ')
//			input++;
//		if(*input)
//			lst = lst->next;
//    }
//    return (lst->next = NULL, first);
//}
