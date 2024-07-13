#include "../../includes/mshell.h"


t_env	*ft_new_env(char *env)
{
	t_env	*new;

	new = malloc(sizeof(t_env));
	new->key = ft_substr(env, 0, ft_strchr(env, '=') - env);
	new->value = ft_strdup(ft_strchr(env, '=') + 1);
	new->next = NULL;
	return (new);
}

void	ft_add_env(t_env **root, t_env *new)
{
	t_env	*tmp;

	tmp = *root;
	if (!*root)
	{
		*root = new;
		return ;
	}
	while (tmp->next)
		tmp = tmp->next;
	tmp->next = new;
}

t_env *extarct_env(char **envp)
{
	int i;
	t_env *env;
	t_env *tmp;

	i = 0;
	env = NULL;
	while (envp[i])
	{
		tmp =ft_new_env(envp[i]);
		ft_add_env(&env, tmp);
		i++;
	}
	return (env);
}