#include "../../includes/mshell.h"

int ft_env(t_cmd cmd, t_env *env)
{
	t_env *tmp;
	(void)cmd;

	tmp = env;
	while (tmp)
	{
		if (tmp->value)
			ft_printf("%s=%s\n", tmp->key, tmp->value);
		tmp = tmp->next;
	}
	return (0);
}

int ft_exit(t_cmd *cmd, t_env *env)
{
	(void)cmd;
	(void)env;
	exit(0);
	return (0);
}

int ft_history(t_cmd *cmd, t_env *env)
{
	//TODO: change params for history t_history *history
	t_history *tmp;

	while (tmp)
	{
		if (tmp->id != 0)
			ft_printf("%d %s\n", tmp->id, tmp->cmd);
		tmp = tmp->next;
	}
	return (0);
}
int echo_arg(char *arg)
{
	int i;

	i = 0;
	if (arg[0] == '-')
	{
		while (arg[i] == 'n')
			i++;
	}
	if (arg[i] == '\0')
		return (1);
	return (0);
}

int ft_echo(t_cmd *cmd, t_env *env)
{
	t_cmd tmp;

	tmp = cmd;
	(void)env;
	if (echo_arg(tmp->arg))
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
	else
	{
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


int ft_unset(t_cmd *cmd, t_env *env)
{
	t_cmd *tmp;
	t_env *tmp_env;

	tmp = cmd;
	while (tmp)
	{
		if (find_env_rem(env, tmp->arg))
			ft_printf("unset: %s: not found\n", tmp->arg);
		tmp = tmp->next;
	}
	return (0);
}



int ft_pwd(t_cmd *cmd, t_env *env)
{
	char *cwd;
	t_env *tmp;

	(void)cmd;
	tmp = find_env(env, "PWD");
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

	tmp = env;
	while (tmp)
	{
		ft_printf("declare -x %s=\"%s\"\n", tmp->key, tmp->value);
		tmp = tmp->next;
	}
}

int ft_export(t_cmd *cmd, t_env *env)
{
	//TODO: implement export
	//TDOD: fix errors
	t_cmd *tmp;

	if (ft_lstsize_bonus(cmd) == 1)
	{
		print_export(env);
		return (0);
	}
	tmp = cmd;
	while (tmp)
	{
		if (ft_strchr(tmp->arg, '='))
		{
			add_env(env, ft_strtok(tmp->arg, "="), ft_strtok(NULL, "="));
		}
		else
		{
			if (find_env(env, tmp->arg))
				ft_printf("export: %s: not a valid identifier\n", tmp->arg);
			else
				add_env(env, tmp->arg, "");
		}
		tmp = tmp->next;
	}
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
int ft_cd(t_cmd *cmd, t_env *env)
{
	//TODO: implement cd
	char *path;

	path = cmd->next->arg;
	if (path == NULL || ft_strcmp(path, "~") == 0)
	{
		path = find_env(env, "HOME")->value;
		if (chdir(path) == -1)
			ft_printf("cd: %s: No such file or directory\n", path);
	}
	else
		cd_cases(path, env);
	add_env(env, "OLDPWD", find_env(env, "PWD")->value);
	edit_env(env, "PWD", getcwd(NULL, 0));
	return (0);
}
 int builtins_finder(char *cmd, t_cmd *cmd, t_env *env)
{
	int flag;
	int i;
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
	while (builtins[i].cmd)
	{
		if (ft_strcmp(builtins[i].cmd, cmd) == 0)
			flag = builtins[i].func(cmd);
			break;
		i++;
	}
	return (flag);
}
