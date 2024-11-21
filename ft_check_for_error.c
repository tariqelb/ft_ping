#include "./ft_ping.h"

int ft_check_for_error(int ac, char **av, int *op)
{
    int i;

    i = 1;

    if (ac == 1)
    {
        printf("ping: missing host operand\n");
        return (-1);
    }
    while (i < ac)
    {
        int equal;
        int help;

        equal = 0;
        help = 0;
        if (av[i][0] == '-' && (ft_strlen(av[i]) > 1))
        {
            equal = ft_strcmp(av[i], "-v");
            help = ft_strcmp(av[i], "-?");
            if (equal != 0 && help != 0)
            {
                if (strncmp(av[i], "-v", 2) == 0)
                {
                    printf("ping: invalid preload value (");
                    int j = 2;
                    while (av[i][j] != '\0')
                    {
                        putchar(av[i][j]);
                        j++;
                    }
                    printf(")\n");
                }
                else
                    printf("ping: invalid option -- \'%c\'\n", av[i][1]);
                return (-1);
            }
            else
                *op = 1;
        }
        i++;
    }
    return (0);
}