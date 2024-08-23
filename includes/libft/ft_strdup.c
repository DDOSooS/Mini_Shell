/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aghergho <aghergho@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/01 15:10:04 by aghergho          #+#    #+#             */
/*   Updated: 2024/06/24 18:33:36 by aghergho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strdup(const char *s)
{
	char	*new;
	int		i;
	int		len;

	len = ft_strlen(s);
	i = 0;
	new = (char *)malloc(sizeof(char) * (len + 1));
	if (!new)
		return (NULL);
	while (i <= len)
	{
		new[i] = s[i];
		i++;
	}
	return (new);
}
/*=====================Testsing part===================
int main ()
{
	char	*s;
	char	*new;

	s = "";
	new = ft_strdup(s);
	if (!new)
		return (0);
	printf("%s\n", new);
	return (0);
}
//=======================================================*/
