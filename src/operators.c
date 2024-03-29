/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   operators.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bnaji <bnaji@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/11 15:02:19 by bnaji             #+#    #+#             */
/*   Updated: 2022/02/03 13:52:43 by bnaji            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	empty_cmd_checker(void)
{
	int	i;

	i = 0;
	if (!*g_data.cmdline)
		return (1);
	while (g_data.cmdline[i] == ' ')
		i++;
	if (!g_data.cmdline[i])
		return (1);
	return (0);
}

/**
 * This function is to check the operators
 * there operators are represented in numbers as following
 * 		1 = |
 *  	2 = >
 * 		3 = <
 * 		4 = ||
 * 		5 = >>
 * 		6 = <<
 * 		7 = &&
 * 		8 = (
 * 		9 = )
 **/
int	operators_checker(int *x, int *ops_cnt, int flag)
{
	if (!g_data.single_qoute_flag && !g_data.double_qoute_flag)
	{
		if (is_single_op(*x) || is_double_op(*x))
		{
			if (is_op(x, ops_cnt, flag))
				return (1);
		}
		else if (g_data.cmdline[(*x)] != ' ')
		{
			if (!flag)
			{
				if (g_data.last_op == 9)
					return (unexpected_msg(*x, 3, NULL));
				g_data.empty_flag = 1;
			}
		}
	}
	else
		return (is_in_qoutes(*x, flag));
	return (0);
}
