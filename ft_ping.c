#include "./ft_ping.h"

//t_list *timer_list = NULL;
struct icmp_and_packet_timer *timer_icmp = NULL;


int	main(int ac, char **av)
{
	int					rand;
	int 				status = 0;
	int 				v_option = 0;
	//int 				socket_fd = 0;
	char 				**destination_addr;
	struct packet		pack;
	struct timeval		timeout;
	//struct icmp_header	*icmp;
	//char				address[100];
	char				resolve_address[100];//host resole to ip to print in the first string

	timer_icmp = NULL;
	timer_icmp = (struct icmp_and_packet_timer *) malloc(sizeof(struct icmp_and_packet_timer));
	if (timer_icmp == NULL)
	{
		printf("Fail to allocate memory\n");
		return (1);
	}
	timer_icmp->icmp = NULL;
	timer_icmp->timer_list = NULL;
	timer_icmp->nbr_of_packet_sent = 0;
	timer_icmp->nbr_of_packet_lost = 0;

	pack.sequence_number = 1;
	
	//icmp = NULL;
	//icmp = malloc(sizeof(struct icmp_header));
	timer_icmp->icmp = NULL;
	timer_icmp->icmp = (struct icmp_header *) malloc(sizeof(struct icmp_header)); 
	if (timer_icmp->icmp == NULL)
	{
		free(timer_icmp);
		printf("Fail to allocate memory\n");
		return (1);
	}


	status = ft_check_parameters(ac, av, &v_option, &destination_addr); 
	if (status)
	{
		free(timer_icmp->icmp);
		free(timer_icmp);
		return (1);
	}
	

	timer_icmp->socket_fd = ft_initialize_socket(&pack, destination_addr);
	if (timer_icmp->socket_fd < 0)
	{
		free(timer_icmp->icmp);
		free(timer_icmp);
		return (1);
	}
	rand = ft_get_random_id(); 
	ft_initialize_icmp_header(&timer_icmp->icmp, &pack, rand);	
	if (destination_addr && destination_addr[0])
	{
		int i = 0;
		while (destination_addr[0][i] != '\0')
		{
			timer_icmp->address[i] = destination_addr[0][i];
			i++;
		}
		timer_icmp->address[i] = '\0';
		ft_free_destination(destination_addr);
	}



	FD_ZERO(&pack.read);
	FD_ZERO(&pack.tmp_read);
	FD_SET(timer_icmp->socket_fd, &pack.read);

	timeout.tv_sec = TIMEOUTSEC;
	timeout.tv_usec = TIMEOUTUSEC;

	//SIGINT signal handler
	signal(SIGINT, ft_signal_handler);
	int sent_data_len = sizeof(pack.send_buffer) - sizeof(struct icmp_header);
	char hex_string[20];
	printf("PING %s (%s) %d data byte", 
		timer_icmp->address,
		timer_icmp->resolved_address,
		sent_data_len);
	if (v_option)
	{
		sprintf(hex_string, "0x%04X", rand);
		printf(", id %s = %d\n", hex_string, rand);
	}
	else
		printf("\n");
	while (1)
	{
		status = sendto(timer_icmp->socket_fd, 
                pack.send_buffer, 
                sizeof(struct icmp_header) + PAYLOADLENGTH,// sizeof(struct icmp_header) + ft_strlen("Hi, this is Ping echo request!"), 
                0, 
                (struct sockaddr*) &pack.packet_address, 
                sizeof(pack.packet_address));
		if (status <= 0)
		{
			printf("ping: sendto: %s\n", strerror(errno));
			continue;
		}
		else
		{
			timer_icmp->nbr_of_packet_sent++;
			timer_icmp->nbr_of_packet_lost++;
			//get current time for the packet and set sequence number
			struct packet_timer new_pack_timer;
			new_pack_timer = ft_initialize_timer(pack.sequence_number);
			if (new_pack_timer.error != 0)
			{
				printf("fail to set time 1");
			}
			t_list *elem = ft_lstnew(&new_pack_timer, sizeof(struct packet_timer));
			if (elem != NULL)
			{
				ft_lstadd_back(&timer_icmp->timer_list, elem);
			}
			else 
			{
				//free and handle the error ;
				free(timer_icmp->icmp);
				ft_lstclear(&timer_icmp->timer_list, &free);
				close(timer_icmp->socket_fd);
				free(timer_icmp);
				printf("Fail to allocate memory\n");
				return (1);
			}
		}
		// printf("packet sent......\n");
		FD_ZERO(&pack.tmp_read);
		pack.tmp_read = pack.read;
		status = select(timer_icmp->socket_fd + 1, &pack.tmp_read, NULL, NULL, &timeout);


		// printf("Status after select : %d\n ", status);

		if (status < 0)
		{
			free(timer_icmp->icmp);
			ft_lstclear(&timer_icmp->timer_list, &free);
			free(timer_icmp);
			printf("Error on select\n");
			return (1);
		}
		else if (status == 0)
		{
			printf("Packet time out\n");
			//timer_icmp->nbr_of_packet_lost++;
			//exit(1);
			ft_set_recv_time(pack.sequence_number, -1);
			pack.sequence_number += 1;
			ft_initialize_icmp_header(&timer_icmp->icmp, &pack, rand);
			sleep(1);
			continue;
		}
		else
		{
			if (FD_ISSET(timer_icmp->socket_fd, &pack.read))
			{
				int len = 0;
				len = recv(timer_icmp->socket_fd, pack.recv_buffer, sizeof(pack.recv_buffer), 0);
				if (len > 0)
				{
					struct iphdr *ip_header = (struct iphdr *) pack.recv_buffer;
					//ip header length in bytes
					int ip_header_len = ip_header->ihl * 4;
					//extract icmp header from the buffer 
					struct icmp_header *icmp_header = (struct icmp_header *) (pack.recv_buffer + ip_header_len);
					//extract payload from the buffer
					char *payload = (char *)(pack.recv_buffer + ip_header_len + sizeof(struct icmp_header));
					
					
					int total_recv_data = len;// - ip_header_len - sizeof(icmp_header);
					//length of sent data
					
					if (icmp_header->type == 0)
					{
						timer_icmp->nbr_of_packet_lost--;
						ft_set_recv_time(pack.sequence_number, len);
						double ms = ft_get_packet_milisec(pack.sequence_number);
						if(ms > EPSILON)
						{
							printf("%d bytes from %s: icmp_seq=%d ttl=%d time=%.3f ms\n",
								total_recv_data,
								inet_ntoa(*(struct in_addr *)&ip_header->saddr),
								pack.sequence_number,
								ip_header->ttl,
								ms);
						}
					}
					else if (v_option)
					{
						ft_print_recv_packet(ip_header, len, icmp_header);
						ft_print_ip_header(ip_header);
						ft_print_icmp_header(timer_icmp->icmp, sizeof(pack.send_buffer));
						ft_set_recv_time(pack.sequence_number, -1);

					}
					else
					{
						//timer_icmp->nbr_of_packet_lost++;
						ft_set_recv_time(pack.sequence_number, -1);
						printf("Fail to get packet time : %d\n", len);
					}
					// if (icmp_header->type == ICMP_ECHOREPLY)
					printf("This is an ICMP Echo . : %d %d recv size: %d\n", icmp_header->type , icmp_header->code, len);

				}
				else if (len <= 0)
				{
					ft_set_recv_time(pack.sequence_number, len);
					//printf("Fail to receive data\n");
					//return (0);
				}
			}
		}
		sleep(1);
		pack.sequence_number += 1;
		ft_initialize_icmp_header(&timer_icmp->icmp, &pack, rand);
	}
	free(timer_icmp->icmp);
	ft_lstclear(&timer_icmp->timer_list, &free);
	free(timer_icmp);
	close(timer_icmp->socket_fd);
	return (0);
}
