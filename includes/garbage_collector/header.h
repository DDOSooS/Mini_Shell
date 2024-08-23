#ifndef HEADER_H
#define HEADER_H

#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>

typedef struct s_list{
	void *content;
	struct s_list *next;
} t_list;

/*======= Garbage Collecotor =======*/
void    ft_lstclear_mshell(t_list **lst);
void	*ft_malloc(size_t size, t_list **list);

#endif // HEADER_H