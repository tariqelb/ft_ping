#include "ft_ping.h"

double  ft_get_time_in_msecond(t_list *elem)
{
    struct packet_timer *p_timer; 
    double              sec_usec;
    double              sent_time;
    double              recv_time;

    p_timer = (struct packet_timer *) elem->content;
    if (p_timer == NULL)
        return (EPSILON);
    if (p_timer->error != 0)
    {
        //printf("Error in timer: %d\n",p_timer->error);
        return (EPSILON);
    }
    sec_usec = 0.0;
    sent_time = p_timer->send_time.tv_sec + p_timer->send_time.tv_usec /  1000000.0;

    recv_time = p_timer->recv_time.tv_sec + p_timer->recv_time.tv_usec /  1000000.0;
    
    //printf("Sent :%d   (%f) (%f)\n",p_timer->seq_nbr, p_timer->send_time.tv_sec, p_timer->send_time.tv_usec);
    //printf("Recv :%d   (%f) (%f)\n", p_timer->seq_nbr, p_timer->recv_time.tv_sec, p_timer->recv_time.tv_usec);
    //printf("Time :%d   recv : %f , sent : %f\n", p_timer->seq_nbr, recv_time, sent_time);
    sec_usec = recv_time - sent_time;
    return (sec_usec * 1000.0);
}


double      ft_get_packet_milisec(int seq_nbr)
{
    struct packet_timer *p_timer;
    t_list *head;
    
    if (timer_list != NULL)
    {
        head = timer_list;
        while (head != NULL)
        {
            p_timer = (struct packet_timer *) head->content;
            if (p_timer && p_timer->seq_nbr == seq_nbr)
            {
                // return (ft_get_time_in_msecond(head)); 
                if (p_timer->error)
                {
                    //printf("Error in timer: %d\n",p_timer->error);
                    return (EPSILON);
                }
                double sec_usec = 0.0;
                double sent_time = p_timer->send_time.tv_sec + p_timer->send_time.tv_usec /  1000000.0;
                double recv_time = p_timer->recv_time.tv_sec + p_timer->recv_time.tv_usec /  1000000.0;
    
                sec_usec = recv_time - sent_time;
                return (sec_usec * 1000.0);
            }
            head = head->next;
        }
    }
    return (EPSILON);
}

int     ft_get_packet_percentage(int nbr_of_packet, int nbr_of_packet_lost)
{
    if (nbr_of_packet == 0)
        return (0);
    if (nbr_of_packet_lost == 0)
        return (0);
    return ((nbr_of_packet_lost * 100) / nbr_of_packet);
}

double ft_get_stddev(int nbr_of_packet, double avg)
{
    if (nbr_of_packet <= 0)
    {
        return (0.0);
    }
    t_list  *head = timer_list;
    double  time_sub_avg_power_two[nbr_of_packet];
    double  time_sum  = EPSILON;
    double  time_avg  = 0.0;
    double  time_sqrt = 0.0;
    int     i = 0;
    int     j = 0;

    while (head)
    {
        time_sub_avg_power_two[i] = ft_get_time_in_msecond(head);
        if (time_sub_avg_power_two[i] != EPSILON)
        {
            double sub = time_sub_avg_power_two[i] - avg;
            time_sub_avg_power_two[i] = pow( sub, (double) 2);
            i++;
        }
        head = head->next;
    }

    while (j < i)
    {
        time_sum += time_sub_avg_power_two[j];
        j++;
    }
    if (time_sum != EPSILON)
        time_avg = time_sum / j;
    time_sqrt = sqrt(time_avg);
    return (time_sqrt);
}

void    ft_nbr_of_packet(int *sent, int *lost)
{
    struct packet_timer *p_time;
    t_list  *head;
    int     i;

    *sent = 0;
    *lost = 0;
    head = timer_list;
    
    i = 0;
    while (head)
    {
        p_time = (struct packet_timer *) head->content;
        if (p_time && p_time->error == 0)
            (*sent)++;
        else if (p_time && p_time->error != 0)
            (*lost)++;        
        head = head->next;
    }
}


void    ft_signal_handler(int a)
{
    int         i;
    int         nbr_of_packet_sent;
    int         nbr_of_packet_lost;
    double      min;
    double      max;
    double      avg;
    double      stddev;

    min = EPSILON;
    max = EPSILON;
    avg = EPSILON;
    stddev = EPSILON;
    i = 0;
    nbr_of_packet_lost = 0;
    nbr_of_packet_sent = 0;
    t_list *head;


    if (timer_list != NULL)
    {
        head = timer_list;
        while (head != NULL)
        {
            double ms = ft_get_time_in_msecond(head);
            if (ms == EPSILON)
                nbr_of_packet_lost++;
            else
            {
                if (i == 0)
                {
                    min = ms; 
                    max = ms;
                }
                if (i != 0 && min > ms)
                    min = ms;
                
                avg += ms;
                
                if (i != 0 && max < ms)
                {
                    max = ms;
                }
            }
            // printf("time taken : %.3f\n", ft_get_time_in_msecond(head));
            //printf("time taken : %.3f\n", ms);
            head = head->next;
            i++;
        }
    }
    ft_nbr_of_packet(&nbr_of_packet_sent, &nbr_of_packet_lost);
    if (avg != EPSILON)
    {
        avg /= (i);
    }
    head = timer_list;
    printf("\n%d packets transmitted, %d packets received, %d%% packet loss\n",
     nbr_of_packet_sent, nbr_of_packet_sent - nbr_of_packet_lost, ft_get_packet_percentage(nbr_of_packet_sent, nbr_of_packet_lost));
    stddev = ft_get_stddev(i - nbr_of_packet_lost, avg);
    printf("round-trip min/avg/max/stddev = %.3f/%.3f/%.3f/%.3f ms\n", min, avg, max, stddev);
    ft_lstclear(&timer_list, &free);
    exit(0);
}