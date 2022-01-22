/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_checker.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bnaji <bnaji@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/30 04:02:06 by bnaji             #+#    #+#             */
/*   Updated: 2022/01/20 00:40:54 by bnaji            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

/**
 * TODO: move the exit function to the parent part not for the child
 * TODO: remove the export and unset function from the child part as well
 * TODO: if the command is not found then exit the child process with 127
 * TODO: add clear command to the execve part
 * TODO: protect check_cmd by checking cmd not cmdline
 * ! remember to dup back the file describtors to not cause errors later on
 * ? Do we need to make sure that child processors are freed before passing them to execve!!
 **/
/*
	This funciton will simply execute the commands using execve 
*/
void	execute_commands(int i)
{
	if (!(ft_strcmp(g_data.cmd[g_data.y][0], "echo")))
		ft_echo();
	else if (!(ft_strcmp(g_data.cmd[g_data.y][0], "env")))
		ft_env();
	else if (!(ft_strcmp(g_data.cmd[g_data.y][0], "pwd")))
		ft_pwd();
	else if (execve(g_data.cmd_path, g_data.cmd[i], g_data.environ) == -1)
	{
		ft_putstr_fd("BNM bash: ", 2);
		ft_putstr_fd(g_data.cmd[i][0], 2);
		ft_putendl_fd(": command not found", 2);
		exit_shell (127);
	}
	exit_shell(0);
}

/*
	this is the last step in the while loop, this function will check the command and execute it after all the redirections, piping are done privously
*/
void	handle_cmd(void)
{
	int	k;

	k = 1;
	if (g_data.closing_parenthese)
	{
		g_data.closing_parenthese = 0;
		return ;
	}
	if (!(ft_strcmp(g_data.cmd[g_data.y][0], "export")))
		while (g_data.cmd[g_data.y][k])
			ft_export(ft_strdup(g_data.cmd[g_data.y][k++]));
	else if (!(ft_strcmp(g_data.cmd[g_data.y][0], "unset")))
		while (g_data.cmd[g_data.y][k])
			ft_unset(ft_strdup(g_data.cmd[g_data.y][k++]));
	else if (!(ft_strcmp(g_data.cmd[g_data.y][0], "cd")))
		ft_cd();
	else if (!(ft_strcmp(g_data.cmd[g_data.y][0], "exit")))
		ft_exit();
	else
	{
		g_data.c_pid = fork();
		save_exit_status();
		if (g_data.c_pid == 0)
		{
			if (g_data.ops_array[g_data.x] == 1)
				close(g_data.fd[g_data.pipes][1]);
			execute_commands(g_data.y);// check for commands and execute them
		}
	}
}

void	check_cmd(void)
{
	int		i;
	int		j;
	int		error_flag;

	i = 0;
	j = 0;
	if (!g_data.cmd)
		return ;
	write(1, BYELLOW, 8);
	while (g_data.cmd[i])
	{
		g_data.y = i;
		g_data.x = j;
		g_data.output_flag = 0;
		g_data.input_flag = 0;
		check_parentheses(&i, &j);
		handle_wild_card(i);
		error_flag = 0;
		if (!g_data.cmd[i])
			break ;
		cmd_filter(g_data.y);
		if (check_op(&i, &j))
			error_flag = 1;
		if (g_data.y != 0)
			pipe_read();
		if (g_data.pipe_flag == 1)
			pipe_write("write2", &i, &j);
		if (!error_flag)
			handle_cmd();
		if (g_data.is_dbl_and)
			check_and_op(&i, &j);
		else if (g_data.is_dbl_pipe)
			check_or_op(&i, &j);
		if (!is_redir(j))
		{
			if (g_data.ops_array[g_data.x] == 1)
				close(g_data.fd[g_data.pipes][1]);
			dup2(g_data.fdin, STDIN_FILENO);
			dup2(g_data.fdout, STDOUT_FILENO);
		}
		wait(NULL);
		if (g_data.sub_exit_flag)
			exit_shell(g_data.exit_status);
	}
	dup2(g_data.fdin, STDIN_FILENO);
	dup2(g_data.fdout, STDOUT_FILENO);
	write(1, NO_COLOR, 4);
}
