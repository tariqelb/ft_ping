/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstnew.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tel-bouh <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/05 17:20:01 by tel-bouh          #+#    #+#             */
/*   Updated: 2021/11/13 20:20:12 by tel-bouh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

t_list	*ft_lstnew(void *content, size_t content_size)
{
	t_list	*list;

	list = (t_list *) malloc(sizeof(t_list));
	if (list == NULL)
		return (NULL);

	if (content != NULL)
	{
		list->content = malloc(content_size);
		if (list->content == NULL)
		{
			free(list);
			return (NULL);
		}
		memcpy(list->content, content, content_size);
	}
	else
		list->content = NULL;

	list->next = NULL;
	return (list);
}
