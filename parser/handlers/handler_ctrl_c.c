/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handler_ctrl_c.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbrol-ca <jbrol-ca@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/02 17:56:44 by hde-barr          #+#    #+#             */
/*   Updated: 2025/04/18 15:12:53 by jbrol-ca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "minishell_part2.h"

void handle_ctrl_c(int sig)
{
	(void)sig;
	write(1,"\n",1);
	rl_replace_line("", 0); // limpa buffer do readline
    rl_on_new_line();
    rl_redisplay();
}

