#include "ft_ping.h"

int ft_strlen(char *str)
{
    if (str == NULL)
        return (-1);
    int i;

    i = 0;
    while (str[i] != '\0')
    {
        i++;
    }
    return (i);
}

int	ft_strcmp(char *s1, char *s2)
{
	int	index;

	index = 0;
	while (s1[index] != '\0' && s2[index] != '\0')
	{
		if (s1[index] == s2[index])
			index++;
		else
			return (s1[index] - s2[index]);
	}
	return (s1[index] - s2[index]);
}

void    ft_free_destination(char **destination)
{
    int i;

    i = 0;
    if (destination == NULL)
        return;
    while (destination[i] != NULL)
    {
        free(destination[i]);
        i++;
    }
    free(destination);
}

void    ft_free_new_destination(char **new_destination, int len)
{
    int i;
    
    if (new_destination == NULL)
        return;
    i = 0;
    while (i < len)
    {
        if (new_destination[i] != NULL)
            free(new_destination[i]);
        new_destination[i] = NULL;
        i++;
    }
    free(new_destination);
    new_destination = NULL;
}

char    **ft_add_address(char **destination_addr, char *addr, int word_nbr)
{
    destination_addr[word_nbr] = (char *) malloc(sizeof(char) * ft_strlen(addr) + 1);
    if (destination_addr[word_nbr] == NULL)
    {
        int k;

        k = 0;
        while (k < word_nbr)
        {
            free(destination_addr[k]);
            destination_addr[k] = NULL;
            k++;
        }
        free(destination_addr);
        destination_addr = NULL;
        return (NULL);
    }
    else
    {
        int j;
        int len;

        j = 0;
        len = ft_strlen(addr);
        while (j < len)
        {
            destination_addr[word_nbr][j] = addr[j];
            j++;
        }
        destination_addr[word_nbr][j] = '\0';
    }
    return (destination_addr);
}

int ft_count_addr(int ac, char **av)
{
    int i = 1;
    int addr = 0;

    while (i < ac)
    {
        if (ft_strlen(av[i]) > 1 && av[i][0] == '-')
            i++;
        else 
        {
            if (ft_strlen(av[i]) > 0)
                addr++;
            i++;
        }
    }
    return (addr);
}

char    **ft_get_destination_address(int ac, char **av)
{
    char **destination;
    int i;
    int nbr_of_addr;


    nbr_of_addr = ft_count_addr(ac, av);
    if (nbr_of_addr == 0)
    {
        printf("ping: missing host operand\n");
        return (NULL);
    }
    destination = NULL;
    destination = (char **) malloc(sizeof(char *) * (nbr_of_addr + 2));
    if (destination == NULL)
        return (NULL);
    destination[nbr_of_addr + 1] = NULL;
    i = 1;
    int word = 0;
    while (i < ac)
    {
        if (ft_strcmp(av[i], "-v") != 0 && ft_strcmp(av[i], "-?") != 0)
        {
            int len;

            len = 0;
            len = ft_strlen(av[i]);//not empty string
            if (len > 0)
            {
                destination = ft_add_address(destination, av[i], word);
                word++;
            }
        }
        i++;
    }
    return (destination);
}