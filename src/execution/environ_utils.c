#include "../../includes/mshell.h"

static void swap_envs(t_env *env1, t_env *env2)
{
	char *tmp_key;
	char *tmp_value;

	tmp_key = env1->key;
	tmp_value = env1->value;
	env1->key = env2->key;
	env1->value = env2->value;
	env2->key = tmp_key;
	env2->value = tmp_value;
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
				swap_envs(tmp, tmp2);
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