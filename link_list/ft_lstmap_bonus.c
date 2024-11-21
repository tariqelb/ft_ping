/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstmap.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tel-bouh <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/06 15:41:04 by tel-bouh          #+#    #+#             */
/*   Updated: 2021/11/13 20:19:54 by tel-bouh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

t_list	*ft_lstmap(t_list *lst, void *(*f)(void *), void (*del)(void *))
{
	void	*data;
	t_list	*map;
	t_list	*new;

	if (lst == NULL || f == NULL || del == NULL)
		return (NULL);
	map = NULL;
	new = NULL;
	data = NULL;
	while (lst)
	{
		data = f(lst->content);
		new = ft_lstnew(data);
		if (new == NULL)
		{
			ft_lstclear(&map, del);
			return (NULL);
		}
		ft_lstadd_back(&map, new);
		lst = lst->next;
	}
	return (map);
}
