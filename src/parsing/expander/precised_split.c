/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   precised_split.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aghergho <aghergho@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/22 11:19:26 by aghergho          #+#    #+#             */
/*   Updated: 2024/08/22 12:01:46 by aghergho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/mshell.h"

int	is_delimter(char c, char *delimiter)
{
	int	i;

	i = -1;
	while (delimiter[++i])
		if (delimiter[i] == c)
			return (1);
	return (0);
}

int	ft_count_word(char *words, char *delimiter)
{
	int	i;
	int	counter;

	i = 0;
	counter = 0;
	while (words[i])
	{
		while (words[i] && is_delimter(words[i], delimiter))
			i++;
		if (words[i])
			counter++;
		while (words[i] && !is_delimter(words[i], delimiter))
			i++;
	}
	return (counter);
}

char	**ft_split_word(char **args, char *words, char *delimiter)
{
	int	i;
	int	j;
	int	k;

	i = 0;
	k = 0;
	while (words[i])
	{
		j = 0;
		while (words[i] && is_delimter(words[i], delimiter))
			i++;
		if (words[i])
		{
			while (words[i + j] && !is_delimter(words[i + j], delimiter))
				j++;
			args[k++] = ft_substr(words, i, j);
			i += j;
		}
	}
	args[k] = NULL;
	return (args);
}

char	**ft_split_words(char *words, char *delimiter)
{
	int		len;
	char	**args;

	len = ft_count_word(words, delimiter);
	args = malloc(sizeof(char *) * (len + 1));
	if (!args)
		return (NULL);
	ft_split_word(args, words, delimiter);
	return (args);
}
