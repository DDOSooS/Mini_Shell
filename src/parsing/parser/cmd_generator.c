# include "../../../includes/mshell.h"



int	ftAddCmd(t_cmd **cmd, char *str)
{
	t_cmd *new;
	t_cmd *tmp;

	new = malloc(sizeof(t_cmd));
	new->arg = ft_strdup(str);
	new->check_wildcard = ftCheckWildCard(new->arg);
	new->next = NULL;
	if (! cmd || !*cmd)
		*cmd = new;

	else
	{
		tmp = *cmd;
		while (tmp->next)
			tmp = tmp->next;
		tmp->next = new;
	}
	return (1);
	
}

int ft_add_to_cmd (t_cmd **root, char *token)
{
	char *cmd;
	t_cmd	*tmp;
	t_cmd	*new;

	tmp = *root;
	cmd = ft_substr(token, 1, ft_strlen(token) - 2);
	free(token);
	new = malloc(sizeof(t_cmd));
	new->arg = cmd;
	new->check_wildcard = 0;
	new->next = NULL;
	if (!root || !*root)
	{
		*root = new;
		return (1);
	}
	while (tmp->next)
		tmp = tmp->next;
	tmp->next = new;
	return (1);
}

t_cmd *ft_gen_new_cmds(char *arg)
{
	t_cmd *new;
	char **cmds;
	int	i;

	i = 0;
	new = NULL;
	cmds = ft_split_words(arg, " \t");
	while (cmds[i])
	{
		ftAddCmd(&new, cmds[i]);
		free(cmds[i]);
		i++;
	}
	free(cmds);
	return (new);
}
t_cmd  *ft_split_cmd( char *arg)
{
	t_cmd	*new;
	
	new = ft_gen_new_cmds(arg);
	free(arg);
	return (new);
}

