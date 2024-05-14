/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mshell.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aghergho <aghergho@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/06 16:01:09 by aghergho          #+#    #+#             */
/*   Updated: 2024/05/14 18:00:38 by aghergho         ###   ########.fr       */
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
    int             is_herdoc;
    int             check_operation;
    int             check_wildcard;
    int             in_group;
    struct  cmd     *next;
} t_cmd;

typedef struct cmd_table
{
    t_cmd                *cmd;
    struct  cmd_table   *next;
} cmd_table;

typedef struct redirection
{
    char                    *in_file;
    char                    *out_file;
    int                     mode;
    struct redirection*     next;
} t_redirection;

enum TokenType
{
    TOKEN_WORD ,
    TOKEN_PIPE,
    TOKEN_IN_REDIRECTION,
    TOKEN_OUT_REDIRECTION,
    TOKEN_LOGICAL_OPERATOR,
    TOKEN_L_PARENTHISE,
    TOKEN_R_PARENTHISE
    
} ;

typedef struct token
{
    char            *type;
    char            *value;
    int             t_number;
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
t_token *ft_tokinizer(char *cmd_line);
int ft_check_syntax(char *cmd_line);
int in_redirection(char c);
int out_redirection(char c);
int is_pipe(char c);
int is_operator(char c);
int is_r_parenthise(char c);
int is_l_parenthise(char c);
int is_whites_space(char c);
int is_charachter(char c);
int ft_check_quote(char *cmd_line, int len);
int is_single_quote(char c);
int is_double_quote(char c);
int is_quote(char c);
// void ft_parse_input(char *cmd);

#endif
