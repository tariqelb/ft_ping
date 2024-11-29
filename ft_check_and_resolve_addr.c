#include "ft_ping.h"

int ft_digit(int digit)
{
    if (digit > 47 && digit < 58)
        return (1);
    return (0);
}

int ft_is_an_ip(char *addr)
{
    int i;

    i = 0;
    while (addr[i] != '\0')
    {
        if (ft_digit(addr[i]) == 0 && addr[i] != '.')
            return (0);
        i++;
    }
    return (1);
}

void    ft_free_split(char **addr)
{
    int i;

    if (addr == NULL)
        return;
    i = 0;
    while (addr[i] != NULL)
    {
        free(addr[i]);
        i++;
    }
    free(addr);
    addr = NULL;
}

int ft_check_is_valid_ip(char *addr)
{
    int     nbr_of_split_word;
    char    **split_addr;
    int     nbr_of_dot;
    int     i;
    int     j;

    nbr_of_dot = 0;
    i = 0;
    while (addr[i] != '\0')
    {
        if (addr[i] == '.')
            nbr_of_dot++;
        i++;
    }
    if (nbr_of_dot != 3)
        return (1);
    nbr_of_split_word = 0;
    split_addr = ft_split(addr, '.');
    i = 0;
    while (split_addr[i] != NULL)
    {
        if (ft_strlen(split_addr[i]) > 3 || ft_strlen(split_addr[i]) <= 0)
        {
                ft_free_split(split_addr);
                return (1);
        }
        j = 0;
        while (split_addr[i][j] != '\0')
        {
            if (ft_digit(split_addr[i][j]) == 0)
            {
                ft_free_split(split_addr);
                return (1);
            }
            j++;
        }
        i++;
        nbr_of_split_word++;
    }
    if (nbr_of_split_word != 4)
        return (1);
    return (0);
}

int     ft_valide_ip_addr(char *addr)
{
    int status;
    int valid;
    struct hostent *host;
    
    status = 0;
    valid = ft_is_an_ip(addr);
    if (valid)
    {
        status = ft_check_is_valid_ip(addr);
    }
    if (valid == 0 || status != 0)
    {
        //resolve host name
        host = gethostbyname(addr);
        if (host == NULL)
        {
            printf("ping: unknown host\n");
            return (1);
        }
        /*addr_list = (struct in_addr **) host->h_addr_list;
        int i = 0;
        while (addr_list[i] != NULL)
        {
            printf("Addr %d %s\n", i, inet_ntoa(*addr_list[i]));
            i++;
        }*/
        //printf("try to resolve this address : %s  %d\n", addr,  status);
    }
    return (0);
}


int     ft_check_and_resolve_addr(char **destination_addr)
{
    int i;
    int status;

    status = 0;
    i = 0;

    if (destination_addr != NULL && destination_addr[i] != NULL)
    {
        status = ft_valide_ip_addr(destination_addr[i]);
    }
    if (status)
    {
        if (destination_addr != NULL)
            return (1);
        while (destination_addr[i] != NULL)
            i++;
        ft_free_new_destination(destination_addr, i);
        return (1);
    }
    return (0);
}


