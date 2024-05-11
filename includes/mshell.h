/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mshell.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aghergho <aghergho@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/06 16:01:09 by aghergho          #+#    #+#             */
/*   Updated: 2024/05/11 20:13:24 by aghergho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MSELL_H
# define MSELL_H

#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <readline/readline.h>
#include <readline/history.h>
#include "./libft/libft.h"
#include "./printf/ft_printf.h"


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

typedef struct cmd
{
    char            *cmd;
    char            **args;
    char            *in_file;
    char            *out_file;
    char            *err_file;
    int             is_herdoc;
    int             check_operation;
    int             check_wildcard;
    struct  cmd     *next;
} t_cmd;

typedef struct cmd_table
{
    t_cmd                *cmd;
    struct  cmd_table   *next;
} cmd_table;

enum TokenType
{
    TOKEN_WORD,
    TOKEN_PIPE,
    TOKEN_REDIRECTION,
    TOKEN_REDIRECT_APPEND,
    TOKEN_LOGICAL_OPERATOR,
    TOKEN_PARENTHISE
} ;

typedef struct token
{
    enum TokenType     type;
    char            *value;
    struct token    *next;
}t_token;


typedef struct mshell
{
 t_env      *env;
 t_history  *history;
 int        exit_value;
} t_mshell;


/*
    minishell_functions
*/
int ft_check_syntax(char *cmd_line);

// void ft_parse_input(char *cmd);

#endif
