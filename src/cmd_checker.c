/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_checker.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bnaji <bnaji@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/30 04:02:06 by bnaji             #+#    #+#             */
/*   Updated: 2022/02/03 19:10:47 by bnaji            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	execute_commands(int i)
{
	if (!(ft_cmdcmp(g_data.cmd[g_data.y][0], "echo")))
		ft_echo();
	else if (!(ft_cmdcmp(g_data.cmd[g_data.y][0], "pwd")))
		ft_pwd();
	else if (!(ft_cmdcmp(g_data.cmd[g_data.y][0], "env")))
		ft_env();
	else
	{
		if (g_data.is_path_flag && access(g_data.cmd_path, R_OK))
			exit_shell(126);
		else if (execve(g_data.cmd_path, g_data.cmd[i], g_data.environ) == -1)
		{
			if (g_data.is_path_flag)
			{
				if (errno == 2)
					print_error(2, i);
			}
			else
				print_error(3, i);
			exit_shell (127);
		}
	}
	exit_shell(0);
}

void	handle_other_commands(void)
{
	if (g_data.cmd_flag && !g_data.pipe_child_flag)
		g_data.c_pid = fork();
	if (g_data.c_pid != 0 && !g_data.pipe_child_flag)
		save_exit_status();
	if (g_data.c_pid == 0)
	{
		if (g_data.ops_array[g_data.x] == 1)
			close(g_data.fd[g_data.pipes][1]);
		execute_commands(g_data.y);
	}
}

void	compare_command(int	*k)
{
	if (!(ft_cmdcmp(g_data.cmd[g_data.y][0], "export")) && g_data.cmd_flag)
	{
		if (g_data.cmd[g_data.y][*k])
		{	
			while (g_data.cmd[g_data.y][*k])
				ft_export(ft_strdup(g_data.cmd[g_data.y][(*k)++]));
		}
		else
			empty_export();
	}
	else if (!(ft_cmdcmp(g_data.cmd[g_data.y][0], "unset")) && g_data.cmd_flag)
		while (g_data.cmd[g_data.y][*k])
			ft_unset(ft_strdup(g_data.cmd[g_data.y][(*k)++]));
	else if (!(ft_cmdcmp(g_data.cmd[g_data.y][0], "cd")) && g_data.cmd_flag)
		ft_cd();
	else if (!(ft_cmdcmp(g_data.cmd[g_data.y][0], "exit")) && g_data.cmd_flag)
		ft_exit();
	else
	{
		handle_other_commands();
	}
}

static int	preparation(int *i, int *j, int *error_flag)
{
	if (setup_for_command(i, j, error_flag))
		return (1);
	if (!g_data.cmd[g_data.x][0] && is_redir(g_data.x))
		*error_flag = 1;
	return (0);
}

/*
	this is the last step in the while loop, this function will check the 
	command and execute it after all the redirections, piping are done privously
*/
void	check_cmd(void)
{
	int		i;
	int		j;
	int		error_flag;

	i = 0;
	j = 0;
	g_data.fd = malloc(sizeof(int *) * (g_data.op_cnt + 5));
	if (!g_data.cmd)
		return ;
	write(1, BYELLOW, 8);
	while (g_data.cmd[i])
	{
		if (preparation(&i, &j, &error_flag))
			break ;
		setup_operators(&i, &j, error_flag);
	}
	dup2(g_data.fdin, STDIN_FILENO);
	dup2(g_data.fdout, STDOUT_FILENO);
	write(1, NO_COLOR, 4);
}
