/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstadd_back.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tel-bouh <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/05 17:42:01 by tel-bouh          #+#    #+#             */
/*   Updated: 2021/11/13 20:17:23 by tel-bouh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_lstadd_back(t_list **lst, t_list *new)
{
	t_list	*head;

	if (new != NULL && lst != NULL)
	{
		head = *lst;
		if (*lst == NULL)
			*lst = new;
		else
		{
			while (head->next != NULL)
				head = head->next;
			head->next = new;
		}
	}
}
