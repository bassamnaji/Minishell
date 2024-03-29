/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parenthesis.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bnaji <bnaji@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/19 16:33:31 by bnaji             #+#    #+#             */
/*   Updated: 2022/02/03 19:10:17 by bnaji            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static int	is_not_paren(int *i, int *j)
{
	if (!g_data.sub_pid)
		return (1);
	else
	{
		if ((g_data.parentheses_cnt == 1 && g_data.was_child)
			|| !g_data.parentheses_cnt)
			return (1);
		else
		{
			(*i)++;
			(*j)++;
			g_data.y = *i;
			g_data.x = *j;
		}
	}
	return (0);
}

static int	is_closed_paren(int *i, int *j)
{
	g_data.paren_cnt++;
	if (!g_data.sub_pid)
	{
		g_data.sub_exit_flag = 1;
		if (g_data.parentheses_cnt == 1)
			return (1);
	}
	else
	{
		(*i)++;
		(*j)++;
		g_data.x = *j;
		g_data.y = *i;
		g_data.parentheses_cnt--;
		g_data.closing_parenthese = 1;
		if (g_data.parentheses_cnt == 0)
		{
			if (g_data.was_child)
				g_data.sub_exit_flag = 1;
			return (1);
		}
	}
	return (0);
}

static void	updata_vars(int *i, int *j)
{
	(*i)++;
	(*j)++;
	g_data.y = *i;
	g_data.x = *j;
	g_data.parentheses_cnt++;
	g_data.paren_cnt++;
}

static int	is_open_paren(int *i, int *j)
{
	if (!g_data.sub_pid)
	{
		g_data.sub_pid = fork();
		waitpid(g_data.sub_pid, &g_data.sub_status, 0);
	}
	if (!g_data.sub_pid)
	{
		if (g_data.paren_array[g_data.paren_cnt] == -2)
		{
			ft_putendl_fd("BnM bash: Syntax Error near `(('", 2);
			exit_shell(1);
		}
		g_data.parentheses_cnt = 0;
		g_data.was_child = 1;
	}
	else
	{
		g_data.exit_status = WEXITSTATUS(g_data.sub_status);
		if (g_data.exit_status == 1)
			error_paren(i, j);
	}
	updata_vars(i, j);
	return (0);
}

int	check_parentheses(int *i, int *j)
{
	while (1)
	{
		if (g_data.ops_array[*j] == 8)
			is_open_paren(i, j);
		else if (g_data.ops_array[*j] == 9)
		{
			if (is_closed_paren(i, j))
				break ;
		}
		else
		{
			if (is_not_paren(i, j))
				break ;
		}
	}
	if (g_data.sub_pid)
		g_data.sub_pid = 0;
	return (0);
}
