#include "../../includes/mshell.h"

int ft_env(t_cmd *cmd, t_mshell *shell)
{
	t_env *tmp;
	(void)cmd;
	tmp = shell->env;
	while (tmp)
	{
		if (tmp->is_exported)
		{
			if (tmp->value)
				ft_printf("%s=%s\n", tmp->key, tmp->value);
			else
				ft_printf("%s=\n", tmp->key);
		}
		tmp = tmp->next;
	}
	ft_printf("_=/usr/bin/env\n");
	return (0);
}

int exit_value_check(char *arg)
{
	int i;

	i = 0;
	if (arg[i] == '-')
		i++;
	while (arg[i])
	{
		if (!ft_isdigit(arg[i]))
			return (0);
		i++;
	}
	return (1);
}

int ft_exit(t_cmd *cmd, t_mshell *shell)
{
	t_cmd *tmp = NULL;
	int exit_value;

	(void)shell;
	if (cmd->next != NULL)
		tmp = cmd->next;
	if (tmp && tmp->next != NULL)
	{
		ft_printf("minishell: exit: too many arguments\n");
		g_mshell.exit_value = 1;
		return (0);
	}
	if (tmp && tmp->arg)
	{
		ft_printf("exit\n");
		if (!exit_value_check(tmp->arg))
		{
			ft_printf("minishell: exit: %s: numeric argument required\n", tmp->arg);
			free_gvar();
			exit(2);
		}
		exit_value = ft_atoi(tmp->arg);
		if (exit_value < 0)
			exit_value = 256 + exit_value;
		free_gvar();
		exit(exit_value);
	}
	else
	{
		free_gvar();
		printf("exit\n");
		exit(g_mshell.exit_value);
	}
	return (0);
}

int ft_history(t_cmd *cmd, t_mshell *shell)
{
	t_history *tmp;
	(void)cmd;
	tmp = shell->history;
	while (tmp)
	{
		ft_printf("%d %s\n", tmp->id, tmp->cmd);
		tmp = tmp->next;
	}
	return (0);
}
int echo_arg(char *arg)
{
	int i;

	i = 0;
	if (arg[i] == '-' && arg[i + 1] == 'n')
	{
		i++;
		while (arg[i] == 'n')
			i++;
	}
	if (arg[i] == '\0')
		return (1);
	return (0);
}

int ft_echo(t_cmd *cmd, t_mshell *shell)
{
	t_cmd *tmp;

	tmp = cmd;
	(void)shell;
	if (tmp->next)
	{
		if (echo_arg(tmp->next->arg))
		{
			tmp = tmp->next->next;
			while (tmp)
			{
				ft_printf("%s", tmp->arg);
				if (tmp->next)
					ft_printf(" ");
				tmp = tmp->next;
			}
			return (0);
		}
		else
		{
			tmp = tmp->next;
			while (tmp)
			{
				ft_printf("%s", tmp->arg);
				if (tmp->next)
					ft_printf(" ");
				tmp = tmp->next;
			}
		}
	}
	ft_printf("\n");
	return (0);
}


int ft_unset(t_cmd *cmd, t_mshell *shell)
{
	t_cmd *tmp;

	tmp = cmd->next;
	while (tmp)
	{
		if (ft_strcmp(tmp->arg, "_") == 0)
			return 0;
		if (!find_env_rem(shell->env, tmp->arg))
			ft_printf("minishell: unset: %s: not found\n", tmp->arg);
		tmp = tmp->next;
	}
	return (0);
}

int ft_pwd(t_cmd *cmd, t_mshell *shell)
{
	char *cwd;
	t_env *tmp;

	(void)cmd;
	tmp = find_env(shell->env, "PWD");
	if (tmp)
		ft_printf("%s\n", tmp->value);
	else
	{
		cwd = getcwd(NULL, 0);
		ft_printf("%s\n", cwd);
		free(cwd);
	}
	return (0);
}


void add_env(t_env *env, char *key, char *value)
{
	t_env *tmp;
	t_env *new;

	tmp = env;
	new = (t_env *)malloc(sizeof(t_env));
	new->key = key;
	new->value = value;
	if (!value)
		new->is_exported = 0;
	else
		new->is_exported = 1;
	new->next = NULL;
	while (tmp->next)
		tmp = tmp->next;
	tmp->next = new;
}


t_env *sort_env(t_env *env)
{
	t_env *tmp;
	t_env *tmp2;
	char *key;
	char *value;

	tmp = env;
	while (tmp)
	{
		tmp2 = tmp->next;
		while (tmp2)
		{
			if (ft_strcmp(tmp->key, tmp2->key) > 0)
			{
				key = tmp->key;
				value = tmp->value;
				tmp->key = tmp2->key;
				tmp->value = tmp2->value;
				tmp2->key = key;
				tmp2->value = value;
			}
			tmp2 = tmp2->next;
		}
		tmp = tmp->next;
	}
	return (env);
}

t_env *copy_env(t_env *env)
{
	t_env *tmp;
	t_env *new;
	t_env *head;

	tmp = env;
	new = (t_env *)malloc(sizeof(t_env));
	if (tmp->key)
		new->key = ft_strdup(tmp->key);
	if (tmp->value)
		new->value = ft_strdup(tmp->value);
	else
		new->value = NULL;
	new->next = NULL;
	head = new;
	tmp = tmp->next;
	while (tmp)
	{
		new->next = (t_env *)malloc(sizeof(t_env));
		new = new->next;
		if (tmp->key)
			new->key = ft_strdup(tmp->key);
		if (tmp->value)
			new->value = ft_strdup(tmp->value);
		else
			new->value = NULL;
		new->next = NULL;
		tmp = tmp->next;
	}
	return (head);
}

void print_export(t_env *env)
{
	t_env *tmp;
	t_env *tmp2;

	tmp = copy_env(env);
	tmp = sort_env(tmp);
	tmp2 = tmp;
	while (tmp)
	{
		if (tmp->value)
			ft_printf("declare -x %s=\"%s\"\n", tmp->key, tmp->value);
		else
			ft_printf("declare -x %s\n", tmp->key);
		tmp = tmp->next;
	}
	free_env(tmp2);
}

int export_checker(char *key)
{
	if (key[0] != '_' && !ft_isalpha(key[0]))
		return (0);
	return (1);
}

int ft_export(t_cmd *cmd, t_mshell *shell)
{
	t_cmd *tmp;
	char *key;
	char *value;

	if (cmd->next == NULL)
	{
		print_export(shell->env);
		return (0);
	}
	tmp = cmd->next;
	while (tmp)
	{
		if (ft_strchr(tmp->arg, '='))
		{
			key = ft_substr(tmp->arg, 0, ft_strchr(tmp->arg, '=') - tmp->arg);
			if (export_checker(key) == 0)
			{
				ft_printf("minishell: export: `%s': not a valid identifier\n", tmp->arg);
				free(key);
				tmp = tmp->next;
				continue;
			}
			value = ft_strdup(ft_strchr(tmp->arg, '=') + 1);
			env_add_back(&shell->env, create_env_node(key, value, 1));
		}
		else
		{
			key = ft_strdup(tmp->arg);
			if (!export_checker(key))
			{
				ft_printf("minishell: export: `%s': not a valid identifier\n", tmp->arg);
				free(key);
				tmp = tmp->next;
				continue;
			}
			env_add_back(&shell->env, create_env_node(key, NULL, 0));
		}
		tmp = tmp->next;
	}
	return (0);
}



int count_args(t_cmd *cmd)
{
	int i;
	t_cmd *tmp;

	i = 0;
	tmp = cmd;
	while (tmp)
	{
		i++;
		tmp = tmp->next;
	}
	return (i);
}
int cd_cases(char *path, t_env *env)
{
	if (path == NULL)
	{
		if (find_env(env, "HOME") == NULL)
		{
			print_stderr("cd: HOME not set");
			g_mshell.exit_value = 1;
			return (0);
		}
		path = find_env(env, "HOME")->value;
	}
	if (chdir(path) == -1)
	{
		print_stderr("cd: no such file or directory");
		g_mshell.exit_value = 1;
		return (0);
	}
	return (1);
}

int ft_cd(t_cmd *cmd, t_mshell *shell)
{
	char *path;
	char buff[1024];
	char *s;

	s = NULL;
	path = NULL;
	if (count_args(cmd) > 2)
	{
		print_stderr("cd: too many arguments");
		g_mshell.exit_value = 1;
		return (0);
	}
	s = getcwd(buff, 1024);
	if (cmd->next)
		path = cmd->next->arg;
	if (cd_cases(path, shell->env))
	{
		if (find_env(shell->env, "OLDPWD"))
			edit_env(shell->env, "OLDPWD", ft_strdup(s));
		if (find_env(shell->env, "PWD"))
			edit_env(shell->env, "PWD", getcwd(NULL, 0));
	}
	return (0);
}

int builtins_checker(t_cmd *cmd)
{
	if (ft_strcmp(cmd->arg, "cd") == 0)
		return (CD);
	if (ft_strcmp(cmd->arg, "echo") == 0)
		return (ECHO);
	if (ft_strcmp(cmd->arg, "pwd") == 0)
		return (PWD);
	if (ft_strcmp(cmd->arg, "export") == 0)
		return (EXPORT);
	if (ft_strcmp(cmd->arg, "unset") == 0)
		return (UNSET);
	if (ft_strcmp(cmd->arg, "env") == 0)
		return (ENV);
	if (ft_strcmp(cmd->arg, "exit") == 0)
		return (EXIT);
	if (ft_strcmp(cmd->arg, "history") == 0)
		return (HISTORY);
	return (-1);
}

int builtins_finder(t_cmd *cmd, t_mshell *shell, int type)
{
	if (type == CD)
		return (ft_cd(cmd, shell));
	if (type == ECHO)
		return (ft_echo(cmd, shell));
	if (type == PWD)
		return (ft_pwd(cmd, shell));
	if (type == EXPORT)
		return (ft_export(cmd, shell));
	if (type == UNSET)
		return (ft_unset(cmd, shell));
	if (type == ENV)
		return (ft_env(cmd, shell));
	if (type == EXIT)
		return (ft_exit(cmd, shell));
	if (type == HISTORY)
		return (ft_history(cmd, shell));
	return (-1);
}
