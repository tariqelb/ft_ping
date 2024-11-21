#include "./ft_ping.h"

int     ft_check_for_help(int ac, char **av)
{
    int i;

    i = 1;
    while (i < ac)
    {
        if (ft_strcmp(av[i], "-?") == 0)
        {
            printf("Usage: ping [OPTION...] HOST ...\n");
            printf("Opetion: -v   verbose output\n");
            printf("Opetion: -?   display this help\n");
            return (-1);
        }
        i++;
    }
    return (0);
}