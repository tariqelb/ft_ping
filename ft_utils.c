#include "./ft_ping.h"

int		ft_get_random_id()
{
	int random;
	
	srand(time(NULL));
	random = 2000 + rand() % (10000 - 2000 + 1);
	return (random);
}

uint16_t compute_checksum(void *data, int len) 
{
    uint32_t sum = 0;
    uint16_t *ptr = (uint16_t *)data;

    for (; len > 1; len -= 2) {
        sum += *ptr++;
    }

    // If there’s a single remaining byte
    if (len == 1) {
        sum += *(uint8_t *)ptr;
    }

    // Fold 32-bit sum to 16 bits
    sum = (sum >> 16) + (sum & 0xFFFF);
    sum += (sum >> 16);

	// printf("ICMP packet prepared with checksum: 0x%x\n", ntohs(~sum));
    return ~sum;
}


struct packet_timer     ft_initialize_timer(int seq_nbr)
{
    struct packet_timer p_timer;
    p_timer.seq_nbr = seq_nbr;
    p_timer.error = 0;
    if (gettimeofday(&p_timer.send_time, NULL) == 0)
    {
        return (p_timer);
    }
    else
    {
        p_timer.error = 1;
    }
        
    return (p_timer);
}

void    ft_set_recv_time(t_list **p_list, int seq_nbr, int len)
{
    // len is the received bytes if <= 0 then an error occur 
    struct packet_timer *p_timer;
    if (p_list == NULL || *p_list == NULL)
        return;
    t_list *head = *p_list; 
    while (head != NULL)
    {
        p_timer = (struct packet_timer *) head->content;
        if (p_timer->seq_nbr == seq_nbr)
        {
            if (len <= 0 || gettimeofday(&(*p_timer).recv_time, NULL) != 0)
            {
                p_timer->error = 1;
            }
            return;
        }   
        head = head->next;
    }
}