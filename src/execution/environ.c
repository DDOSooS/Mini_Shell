#include "../../includes/mshell.h"
t_env *find_env(t_env *env, char *key)
{
	t_env *tmp;

	tmp = env;
	while (tmp)
	{
		if (ft_strcmp(tmp->key, key) == 0)
			return (tmp);
		tmp = tmp->next;
	}
	return (NULL);
}
int find_env_rem(t_env *env, char *key)
{
	t_env *prev;
    t_env *current;

	prev = NULL;
	current = env;
    while (current != NULL)
	{
        if (ft_strcmp(current->key, key) == 0)
		{
            if (prev == NULL) 
                env = current->next;
            else
                prev->next = current->next; 
            free(current->key);
            free(current->value);
            free(current);
            return 1;
        }
        prev = current;
        current = current->next;
    }
    return 0;
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
		tmp = (t_env *)malloc(sizeof(t_env));
		tmp->key = ft_substr(envp[i], 0, ft_strchr(envp[i], '=') - envp[i]);
		tmp->value = ft_strdup(ft_strchr(envp[i], '=') + 1);
		tmp->next = env;
		env = tmp;
		i++;
	}
	//TODO: remove the oldpwd
	find_env_rem(env, "OLDPWD");
	return (env);
}