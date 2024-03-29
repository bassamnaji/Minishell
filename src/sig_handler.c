/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sig_handler.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bnaji <bnaji@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/14 11:17:55 by bnaji             #+#    #+#             */
/*   Updated: 2022/02/02 12:05:43 by bnaji            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	sigquit_handler(void)
{
	if (g_data.c_pid)
	{
		kill(g_data.c_pid, SIGQUIT);
		write(0, "\n", 1);
		g_data.exit_status = 131;
		g_data.c_exit_flag = 1;
	}
	else
	{
		rl_on_new_line();
		rl_redisplay();
		ft_putstr_fd("  \r", 2);
		rl_on_new_line();
		rl_redisplay();
	}
}

void	sigint_handler(void)
{
	if (g_data.c_pid)
	{
		kill(g_data.c_pid, SIGINT);
		write(1, "\n", 1);
		g_data.exit_status = 130;
		g_data.c_exit_flag = 1;
	}
	else
	{
		rl_on_new_line();
		rl_redisplay();
		write(0, "  ", 2);
		rl_replace_line("", 0);
		write(0, "\n", 1);
		rl_on_new_line();
		rl_redisplay();
		g_data.exit_status = 1;
	}
}

/**
 */
void	sig_handler(int signum)
{
	if (signum == SIGQUIT)
		sigquit_handler();
	else
		sigint_handler();
}
