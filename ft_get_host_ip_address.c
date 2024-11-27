#include "ft_ping.h"

char    *ft_get_host_ip_address(char **destination)
{
    int     i;
    int     status;
    int     len;
    // char    *host_address;

    i = 0;
    status = 0;
    timer_icmp->resolved_address = NULL;
    status = ft_check_is_valid_ip(destination[0]);
    if (status == 0)
    {
        len = ft_strlen(destination[0]);
        timer_icmp->resolved_address = (char *) malloc(sizeof(char) * len + 1);
        if (timer_icmp->resolved_address == NULL)
        {
            printf("Fail to allocate memory\n");
            return (NULL);
        }
        while (i < len)
        {
            timer_icmp->resolved_address[i] = destination[0][i];
            i++;
        }
        timer_icmp->resolved_address[i] = '\0';
        return (timer_icmp->resolved_address);
    }
    else
    {
        int len;
        struct hostent *host;
        struct in_addr **addr_list;
        host = gethostbyname(destination[0]);
        if (host == NULL)
        {
            return (NULL);
        }
        addr_list = (struct in_addr **) host->h_addr_list;
        //addr_list can hold multiple address for one hostname, i took the last one
        i = 0;
        while (addr_list[i] != NULL)
        {
            char *temp = inet_ntoa(*addr_list[i]);
            len = ft_strlen(temp);
            i++;
        }
        if (len)
        {
            timer_icmp->resolved_address = (char *) malloc(sizeof(char) * len + 1);
            if (timer_icmp->resolved_address == NULL)
            {
                printf("Fail to allocate memory\n");
                return (NULL);
            } 
            i = 0;
            while (addr_list[i] != NULL)
            {
                char *temp = inet_ntoa(*addr_list[i]);
                if (addr_list[i + 1] == NULL)
                {
                    len = 0;
                    while (temp[len] != 0)
                    {
                        timer_icmp->resolved_address[len] = temp[len];
                        len++;
                    }   
                }
                i++;
            }
        }
        return (timer_icmp->resolved_address);
    }
}