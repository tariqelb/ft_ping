/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstclear.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tel-bouh <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/06 14:13:36 by tel-bouh          #+#    #+#             */
/*   Updated: 2021/11/13 20:18:12 by tel-bouh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_lstclear(t_list **lst, void (*del)(void *))
{
	t_list	*head;
	t_list	*tmp;

	head = NULL;
	tmp = NULL;
	if (del != NULL && lst != NULL)
	{
		head = *lst;
		while (head)
		{
			tmp = head->next;
			del(head->content);
			free(head);
			head = tmp;
		}
		*lst = NULL;
	}
}
