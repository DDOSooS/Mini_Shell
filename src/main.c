/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aghergho <aghergho@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/06 16:02:56 by aghergho          #+#    #+#             */
/*   Updated: 2024/08/28 22:56:13 by aghergho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/mshell.h"

t_mshell	g_mshell;

void	m_shell_init(char **envp)
{
	g_mshell.cmd_tree = NULL;
	g_mshell.token = NULL;
	g_mshell.pid = get_pid();
	g_mshell.herdocs = NULL;
	g_mshell.n_herdoc = 0;
	g_mshell.flag = 1;
	g_mshell.n_herdoc_executed = 0;
	g_mshell.exit_value = 0;
	extarct_env(envp, &g_mshell.env);
	g_mshell.history = NULL;
	g_mshell.history = (t_history *)malloc(sizeof(t_history));
	g_mshell.history->id = 0;
	g_mshell.history->cmd = NULL;
	g_mshell.history->next = NULL;
}

char	*costum_readline(void)
{
	char	*line;
	char	curr[2024];
	char	*path;
	char	*tmp;

	if (getcwd(curr, 2024) != NULL)
	{
		tmp = ft_strjoin(CYAN "minishell" RESET GREEN "$>" RESET, curr);
		path = ft_strjoin(tmp, ":$ ");
		free(tmp);
	}
	else
		path = ft_strdup(CYAN "minishell" RESET GREEN "$>" RESET ".:$ ");
	if (check_tty())
		line = readline(path);
	else
		line = get_next_line(STDIN_FILENO);
	free(path);
	if (!line)
	{
		if (check_tty())
			ft_putstr_fd("exit\n", 1);
		(free_gvar(1), exit(0));
	}
	return (line);
}

void	ft_handle_history(char **cmd_line)
{
	if (check_tty() && *cmd_line && check_white_spaces(*cmd_line))
	{
		add_history(*cmd_line);
		put_tohistory(*cmd_line, g_mshell.history, 0);
	}
	if (ft_check_comments(*cmd_line))
		ft_handle_comment(cmd_line);
}

void	ft_execute_cli(void)
{
	if (ft_expand_tokens(&g_mshell.token, 1))
	{
		ft_parse_ast(&g_mshell.cmd_tree, &g_mshell.token);
		g_mshell.herdocs = ft_gen_herdocs(g_mshell.token);
		execute(g_mshell.cmd_tree, &g_mshell);
		ft_free_cmd_var();
	}
	else
		ft_free_tokens(&g_mshell.token);
}

int	main(int ac, char **av, char **envp)
{
	char	*cmd_line;

	cmd_line = NULL;
	((void)ac, (void)av);
	m_shell_init(envp);
	while (1)
	{
		handle_signals(interactive_sigint, SIG_IGN, SIG_IGN, SIG_IGN);
		cmd_line = costum_readline();
		ft_handle_history(&cmd_line);
		if (!ft_check_syntax(cmd_line))
		{
			free(cmd_line);
			if (check_tty())
				continue ;
			return (free_gvar(1), EXIT_FAILURE);
		}
		g_mshell.token = ft_tokinizer(cmd_line);
		if (g_mshell.token)
			ft_execute_cli();
		free(cmd_line);
	}
	return (EXIT_SUCCESS);
}
