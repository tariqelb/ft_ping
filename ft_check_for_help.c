#include "./ft_ping.h"

int     ft_check_for_help(int ac, char **av)
{
    int i;

    i = 1;
    while (i < ac)
    {
        if (ft_strcmp(av[i], "-h") == 0)
        {
            printf("Usage: ping [OPTION...] HOST ...\n");
            printf("Option: -v   verbose output\n");
            printf("Option: -h   display this help\n");
            return (-1);
        }
        i++;
    }
    return (0);
}