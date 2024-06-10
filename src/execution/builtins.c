#include "../../includes/mshell.h"

int ft_env(t_cmd *cmd, t_mshell *shell)
{
	printf("env, cmd %s\n", cmd->arg);
	t_env *tmp;
	(void)cmd;
	tmp = shell->env;
	printf("======= env =======\n");
	while (tmp)
	{
		ft_printf("%s=%s\n", tmp->key, tmp->value);
		tmp = tmp->next;
	}
	printf("======= end env =======\n");
	return (0);
}

int ft_exit(t_cmd *cmd, t_mshell *shell)
{
	printf("exit, cmd %s\n", cmd->arg);
	(void)cmd;
	(void)shell;
	exit(0);
	return (0);
}

int ft_history(t_cmd *cmd, t_mshell *shell)
{
	printf("history, cmd %s\n", cmd->arg);
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
	if (arg[i] == '-')
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
	printf("echo, cmd %s\n", cmd->arg);
	t_cmd *tmp;

	tmp = cmd;
	(void)shell;
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
		ft_printf("\n");
	}
	return (0);
}


int ft_unset(t_cmd *cmd, t_mshell *shell)
{
	t_cmd *tmp;

	tmp = cmd->next;
	while (tmp)
	{
		if (!find_env_rem(shell->env, tmp->arg))
			ft_printf("unset: %s: not found\n", tmp->arg);
		tmp = tmp->next;
	}
	return (0);
}

int ft_pwd(t_cmd *cmd, t_mshell *shell)
{
	printf("pwd, cmd %s\n", cmd->arg);
	char *cwd;
	t_env *tmp;

	(void)cmd;
	printf("here(1)\n");
	tmp = find_env(shell->env, "PWD");
	printf("here(22\n");
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
	new->next = NULL;
	while (tmp->next)
		tmp = tmp->next;
	tmp->next = new;
}
void edit_env(t_env *env, char *key, char *value)
{
	t_env *tmp;

	tmp = find_env(env, key);
	if (tmp)
	{
		free(tmp->value);
		tmp->value = value;
	}
	else
		add_env(env, key, value);
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

void print_export(t_env *env)
{
	t_env *tmp;

	tmp = sort_env(env);
	while (tmp)
	{
		ft_printf("declare -x %s=\"%s\"\n", tmp->key, tmp->value);
		tmp = tmp->next;
	}
}

int ft_export(t_cmd *cmd, t_mshell *shell)
{
	t_cmd *tmp;

	if (cmd->next == NULL)
	{
		//FIXME: print all env SORTED and not edit the env
		print_export(shell->env);
		return (0);
	}
	tmp = cmd->next;
	while (tmp)
	{
		printf("tmp->arg %s\n", tmp->arg);
		if (ft_strchr(tmp->arg, '='))
		{
			edit_env(shell->env, ft_substr(tmp->arg, 0, ft_strchr(tmp->arg, '=') - tmp->arg), ft_strdup(ft_strchr(tmp->arg, '=') + 1));
		}
		else
			edit_env(shell->env, tmp->arg, "");
		tmp = tmp->next;
	}
	return (0);
}

void cd_cases(char *path, t_env *env)
{
	if (ft_strcmp(path, "-") == 0)
	{
		path = find_env(env, "OLDPWD")->value;
		if (path == NULL)
			ft_printf("cd: OLDPWD not set\n");
		if (chdir(path) == -1)
			ft_printf("cd: %s: No such file or directory\n", path);
	}
	if (chdir(path) == -1)
		ft_printf("cd: %s: No such file or directory\n", path);
}
int ft_cd(t_cmd *cmd, t_mshell *shell)
{
	printf("cd, cmd %s\n", cmd->arg);
	//TODO: implement cd
	char *path;

	path = cmd->next->arg;
	if (path == NULL || ft_strcmp(path, "~") == 0)
	{
		path = find_env(shell->env, "HOME")->value;
		if (chdir(path) == -1)
			ft_printf("cd: %s: No such file or directory\n", path);
	}
	else
		cd_cases(path, shell->env);
	add_env(shell->env, "OLDPWD", find_env(shell->env, "PWD")->value);
	edit_env(shell->env, "PWD", getcwd(NULL, 0));
	// add_env(env, "OLDPWD", find_env(env, "PWD")->value);
	// edit_env(env, "PWD", getcwd(NULL, 0));
	return (0);
}
 int builtins_finder(t_cmd *cmd, t_mshell *shell)
{
	int flag;
	int i;
	printf("builtins_finder, cmd %s\n", cmd->arg);
	t_builtins builtins[] = {
		{"cd", &ft_cd},
		{"echo", &ft_echo},
		{"pwd", &ft_pwd},
		{"export", &ft_export},
		{"unset", &ft_unset},
		{"env", &ft_env},
		{"exit", &ft_exit},
		{"history", &ft_history},
		{NULL, NULL}
	};

	i = 0;
	flag = -1;
	while (builtins[i].name)
	{
		if (ft_strcmp(builtins[i].name, cmd->arg) == 0)
		{
			flag = builtins[i].func(cmd, shell);
			break;
		}
		i++;
	}
	return (flag);
}
