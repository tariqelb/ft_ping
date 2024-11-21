#include "ft_ping.h"


double  ft_get_time_in_second(t_list *elem)
{
    struct packet_timer *p_timer; 
    double              sec_usec;
    double              sent_time;
    double              recv_time;

    p_timer = (struct packet_timer *) elem->content;
    if (p_timer == NULL || p_timer->error)
        return (0);
    sec_usec = 0;
    sent_time = p_timer->send_time.tv_sec;
    if (p_timer->send_time.tv_usec > 0)
        sent_time += p_timer->send_time.tv_usec / 1000;
    recv_time = p_timer->recv_time.tv_sec;
    if (p_timer->recv_time.tv_usec > 0)
        recv_time += p_timer->recv_time.tv_usec / 1000;

    sec_usec = recv_time - sent_time;
    return (sec_usec);
}

void    ft_signal_handler(int a)
{
    double      min;
    double      max;
    double      avg;
    double      stddev;

    min = 0;
    max = 0;
    avg = 0;
    stddev = 0;

    t_list *head;

    printf("Handler called ......\n");

    if (timer_list != NULL)
    {
        head = timer_list;
        while (head != NULL)
        {
            printf("time taken : %f\n", ft_get_time_in_second(head));
            head = head->next;
        }
    }
}