#include "ft_ping.h"

int     ft_check_parameters(int ac, char **av, int *v_option, char ***destination_addr)
{
    int status;

    // check for argument error
	status = ft_check_for_error(ac, av, v_option);
	if (status == -1)
		return (0);
	//check for -? help
	status = ft_check_for_help(ac, av); 
	if (status == -1)
		return (0);
	//Get destination address
	*destination_addr = ft_get_destination_address(ac, av);
	if (destination_addr == NULL)
	{
		printf("Fail to allocate memory\n");
		return (0);
	}
	int i = 0;
	while (destination_addr != NULL && destination_addr[0] != NULL && destination_addr[0][i] != NULL)
	{
		printf("Destination address  i %d %s\n", i, *destination_addr[i]);
		i++;
	}
	status = ft_check_and_resolve_addr(*destination_addr);
}