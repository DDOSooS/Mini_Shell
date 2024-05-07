/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mshell.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aghergho <aghergho@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/06 16:01:09 by aghergho          #+#    #+#             */
/*   Updated: 2024/05/07 14:25:53 by aghergho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MSELL_H
# define MSELL_H

#include <unistd.h>
#include <stdlib.h>
#include "./libft/libft.h"
#include "./printf/ft_printf.h"
#include <readline/readline.h>
#include <readline/history.h>
/*
    get_next_line functions declaration
*/

char	*get_next_line(int fd);
int		check_end_line(char *str);
char	*str_sub(char *str, int len);
/*
    end of gnl funtion's declaration
*/

typedef struct  env
{
    char        *key;
    char        *value;
    struct env  *next;
} t_env;

typedef struct history
{
    int             id;
    char            *cmd;
    struct history  *next;
} t_history;

typedef struct mshell
{
 t_env      *env;
 t_history  *history;
 int        exit_value;
} t_mshell;

#endif
