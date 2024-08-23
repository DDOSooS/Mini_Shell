/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aghergho <aghergho@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/01 16:13:15 by aghergho          #+#    #+#             */
/*   Updated: 2023/11/12 23:38:17 by aghergho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strjoin(char const *s1, char const *s2)
{
	char	*str;
	size_t	len;

	if (!s1 && !s2)
		return (0);
	if (!s1)
		return (ft_strdup(s2));
	if (!s2)
		return (ft_strdup(s1));
	len = ft_strlen(s1) + ft_strlen(s2);
	str = (char *)malloc(sizeof(char) * (len + 1));
	if (!str)
		return (0);
	ft_strlcpy(str, s1, ft_strlen(s1) + 1);
	ft_strlcat(str, s2, len + 1);
	return (str);
}

/*=====================TESTING PART==================
#include <stdio.h>

int	main(void)
{
	char	*str1;
	char	*str2;
	char	*str3;

	str1 = "Hello";
	str2 = " World";
	str3 = ft_strjoin(str1, str2);
	printf("%s\n", str3);
	free(str3);
	return (0);
}
=====================================================*/
