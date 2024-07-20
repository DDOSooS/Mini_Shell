
#include "../../../includes/mshell.h"

int	export_checker(char *key)
{
	if (key[0] != '_' && !ft_isalpha(key[0]))
		return (0);
	return (1);
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

static void	print_export(t_env *env)
{
	t_env	*tmp;
	t_env	*tmp2;

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

int	ft_export(t_cmd *cmd, t_mshell *shell)
{
	//TODO: handle more cases of this kind of export
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
				ft_printf("minishell: export: `%s': not a valid identifier\n",
					tmp->arg);
				free(key);
				tmp = tmp->next;
				continue ;
			}
			value = ft_strdup(ft_strchr(tmp->arg, '=') + 1);
			env_add_back(&shell->env, create_env_node(key, value, 1));
		}
		else
		{
			key = ft_strdup(tmp->arg);
			if (!export_checker(key))
			{
				ft_printf("minishell: export: `%s': not a valid identifier\n",
					tmp->arg);
				free(key);
				tmp = tmp->next;
				continue ;
			}
			env_add_back(&shell->env, create_env_node(key, NULL, 0));
		}
		tmp = tmp->next;
	}
	return (0);
}
