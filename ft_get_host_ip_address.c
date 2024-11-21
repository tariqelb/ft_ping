#include "ft_ping.h"

char    *ft_get_host_ip_address(char **destination)
{
    int     i;
    int     status;
    int     len;
    char    *host_address;

    i = 0;
    status = 0;
    host_address = NULL;
    status = ft_check_is_valid_ip(destination[0]);
    if (status == 0)
    {
        len = ft_strlen(destination[0]);
        host_address = (char *) malloc(sizeof(char) * len + 1);
        if (host_address == NULL)
        {
            return (NULL);
        }
        while (i < len)
        {
            host_address[i] = destination[0][i];
            i++;
        }
        host_address[i] = '\0';
        return (host_address);
    }
    else
    {
        struct hostent *host;
        struct in_addr **addr_list;
        host = gethostbyname(destination[0]);
        if (host == NULL)
        {

            return (NULL);
        }
        addr_list = (struct in_addr **) host->h_addr_list;
        i = 0;
        while (addr_list[i] != NULL)
        {
            host_address = inet_ntoa(*addr_list[i]);
            i++;
        }
        return (host_address);
    }
}