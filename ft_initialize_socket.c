#include "ft_ping.h"

int     ft_initialize_socket(struct packet *pack, char **destination_addr)
{
    int     broadcast;
    int     socket_fd;
    int     set_opt;
    int     ttl;
    int     i;

    broadcast = 1;
    ttl = TIMETOLIVE;
    socket_fd = -1;
    set_opt = -1;
    i = 0;
    
    //initialize socket
    socket_fd = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
	if (socket_fd < 0)
	{
		printf("Error opening the socket: (%d)\n", socket_fd);
		return (-1);
	}

	
	//set max hop (router) that packet will made before discard 
	set_opt = setsockopt(socket_fd, IPPROTO_IP, IP_TTL, &ttl, sizeof(ttl));
	if (set_opt != 0)
	{
		printf("Error setting time-to-live ttl");
		return (-1);
	}
	//set allow sending broadcast packet (why, goal)
	set_opt = setsockopt(socket_fd, SOL_SOCKET, SO_BROADCAST, &broadcast, sizeof(broadcast));
	if (set_opt != 0)
	{
		printf("Error setting broadcast");
		return (-1);
	}	
	pack->host_ip_address = ft_get_host_ip_address(destination_addr);
	if (pack->host_ip_address == NULL)
	{
		if (destination_addr == NULL)
			return (-1);
		while (destination_addr[i] != NULL)
			i++;
		ft_free_new_destination(destination_addr, i);
		return (-1);
	}
	memset(&pack->packet_address, 0, sizeof(pack->packet_address));
	pack->packet_address.sin_family = AF_INET;
	inet_pton(AF_INET, pack->host_ip_address, &pack->packet_address.sin_addr);
    return (socket_fd);
}