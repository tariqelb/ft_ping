#include "./ft_ping.h"

t_list *timer_list = NULL;

int	main(int ac, char **av)
{
	int 				status = 0;
	int 				v_option = 0;
	int 				socket_fd = 0;
	char 				**destination_addr;
	struct packet		pack;
	struct timeval		timeout;
	struct icmp_header	*icmp;
	int					ping_first_string_flag;


	ping_first_string_flag = 0;
	pack.sequence_number = 1;
	icmp = NULL;
	icmp = malloc(sizeof(struct icmp_header));
	if (icmp == NULL)
	{
		printf("Fail to allocate memory\n");
		return (0);
	}


	status = ft_check_parameters(ac, av, &v_option, &destination_addr); 
	if (status)
		return (0);
	//return (0);
	

	socket_fd = ft_initialize_socket(&pack, destination_addr);
	ft_initialize_icmp_header(&icmp, &pack);	




	FD_ZERO(&pack.read);
	FD_ZERO(&pack.tmp_read);
	FD_SET(socket_fd, &pack.read);

	timeout.tv_sec = TIMEOUTSEC;
	timeout.tv_usec = TIMEOUTUSEC;

	//SIGINT signal handler
	signal(SIGINT, ft_signal_handler);

	while (1)
	{
		status = sendto(socket_fd, 
                pack.send_buffer, 
                sizeof(struct icmp_header) + PAYLOADLENGTH,// sizeof(struct icmp_header) + ft_strlen("Hi, this is Ping echo request!"), 
                0, 
                (struct sockaddr*) &pack.packet_address, 
                sizeof(pack.packet_address));
		if (status <= 0)
		{
			printf("ping: sendto: Network is unreachable\n");
			continue;
		}
		else
		{
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
				ft_lstadd_back(&timer_list, elem);
			}
			else 
			{
				//free and handle the error ;
				ft_free_destination(destination_addr);
				printf("Fail to allocate memory\n");
				return (0);
			}
		}
		// printf("packet sent......\n");
		FD_ZERO(&pack.tmp_read);
		pack.tmp_read = pack.read;
		status = select(socket_fd + 1, &pack.tmp_read, NULL, NULL, &timeout);


		// printf("Status after select : %d\n ", status);

		if (status < 0)
		{
			printf("Error on select\n");
			ft_free_destination(destination_addr);
			return (0);
		}
		else if (status == 0)
		{
			printf("Packet time out\n");
			//exit(1);
			ft_set_recv_time(pack.sequence_number, -1);
			pack.sequence_number += 1;
			ft_initialize_icmp_header(&icmp, &pack);
			sleep(1);
			continue;
		}
		else
		{
			if (FD_ISSET(socket_fd, &pack.read))
			{
				int len = 0;
				len = recv(socket_fd, pack.recv_buffer, sizeof(pack.recv_buffer), 0);
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
					int sent_data_len = sizeof(pack.send_buffer) - sizeof(struct icmp_header);
					if (!ping_first_string_flag)	
					{
						ping_first_string_flag = 1;
						printf("PING %s (%s) %d data byte\n", 
						destination_addr[0],
						inet_ntoa(*(struct in_addr *)&ip_header->saddr),
						sent_data_len);
					}
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
					else
					{
						printf("Fail to get packet time : %d\n", len);
					}
					// if (icmp_header->type == ICMP_ECHOREPLY)
					// 	printf("This is an ICMP Echo Reply.\n");

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
		ft_initialize_icmp_header(&icmp, &pack);
	}
	ft_free_destination(destination_addr);
	close(socket_fd);
	return (0);
}
