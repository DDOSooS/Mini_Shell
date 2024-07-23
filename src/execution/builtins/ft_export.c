#include "../../../includes/mshell.h"

int export_erorr(char *arg, int status)
{
	write(2, "msh: export: `", 14);
	write(2, arg, ft_strlen(arg));
	write(2, "': not a valid identifier\n", 27);
	return (status);
}

int	export_checker(t_env *new, char *arg)
{
	int i;
	int plus;

	i = 0;
	plus = 0;
	while (new && new->key[i])
	{
		if (!ft_isalnum(new->key[i]) && new->key[i] != '_' && new->key[i] != '+')
			return (export_erorr(new->key, 1));
		if (new->key[i] == '+')
			plus++;
		if (plus > 1 || new->key[0] == '+' || (new->key[0] >= '0' && new->key[0] <= '9'))
			return (export_erorr(new->key, 1));
		i++;
	}
	if ((new->key && new->key[0] == '\0') || (new->key[ft_strlen(new->key) - 1] == '+' && new->value == NULL) || (plus == 1 && new->key[ft_strlen(new->key) - 1] != '+'))
		return (export_erorr(new->key, 1));
	
	return (0);
}

t_env	*sort_env(t_env *env)
{
	t_env	*tmp;
	t_env	*tmp2;
	char	*key;
	char	*value;

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

t_env	*copy_env(t_env *env)
{
	t_env	*tmp;
	t_env	*new;
	t_env	*head;

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

static int print_export(t_env *env)
{
	t_env	*tmp;
	t_env	*tmp2;

	tmp = copy_env(env);
	tmp = sort_env(tmp);
	tmp2 = tmp;
	while (tmp)
	{
		if (ft_strcmp(tmp->key, "_") != 0)
		{
			if (tmp->value)
				printf("declare -x %s=\"%s\"\n", tmp->key, tmp->value);
			else
				printf("declare -x %s\n", tmp->key);
		}
		tmp = tmp->next;
	}
	free_env(tmp2);
	return (0);
}

t_env *extarct_node(char *args)
{
	t_env *new;
	char *key;
	char *value;
	char *tmp;

	tmp = ft_strchr(args, '=');
	if (tmp == NULL)
		new = create_env_node(ft_strdup(args), NULL, 0);
	else
	{
		key = ft_substr(args, 0, tmp - args);
		value = ft_strdup(tmp + 1);
		new = create_env_node(key, value, 1);
	}
	return (new);
}

void appned_export(t_env *env, t_env *new)
{
	t_env *tmp;
	t_env *found_env;
	char *tmp_value;

	new->key[ft_strlen(new->key) - 1] = '\0';
	found_env = find_env(env, new->key);
	tmp = env;
	if (found_env == NULL)
	{
		env_add_back(&env, new);
		return ;
	}
	else if (new->value != NULL)
	{
		tmp_value = ft_strjoin(found_env->value, new->value);
		free(found_env->value);
		found_env->value = tmp_value;
		free_env(new);
	}
	else
		free_env(new);
}

void replacement_export(t_env *env, t_env *new)
{
	t_env *tmp;
	t_env *tmp2;
	t_env *found_env;

	found_env = find_env(env, new->key);
	tmp = env;
	tmp2 = env->next;
	if (found_env == NULL)
	{
		env_add_back(&env, new);
		return ;
	}
	else if (new->value != NULL)
	{
		if (found_env->value)
			free(found_env->value);
		found_env->value = ft_strdup(new->value);
		free_env(new);
	}
	else
		free_env(new);
}

void add_append_env(t_env *env, t_env *new)
{
	if (ft_strchr(new->key, '+'))
		appned_export(env, new);
	else
		replacement_export(env, new);
}

int add_export(t_cmd *cmd, t_mshell *shell)
{
	t_env *new;
	int i;
	int status;

	i = 1;
	status = 0;
	while (cmd && cmd->arg && cmd->arg[0])
	{
		new = extarct_node(cmd->arg);
		if ((status = export_checker(new, cmd->arg)))
		{
			free_env(new);
			cmd = cmd->next;
			continue;
		}
		add_append_env(shell->env, new);
		cmd = cmd->next;
	}
	return (status);
}

int	ft_export(t_cmd *cmd, t_mshell *shell)
{
	//TODO: handle more cases of this kind of export
	t_cmd *tmp;

	if (cmd->next == NULL)
		return (print_export(shell->env));
	tmp = cmd->next;
	return (add_export(tmp, shell));
}
