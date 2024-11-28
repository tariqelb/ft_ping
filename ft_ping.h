#ifndef FT_PING_H
# define  FT_PING_H

# include <stdio.h>
# include <unistd.h>
# include <sys/time.h>
# include <sys/select.h>
# include <sys/types.h>
# include <sys/socket.h>
# include <netinet/ip.h>
# include <netinet/in.h>
# include <string.h>
# include <stdlib.h>
# include <netdb.h>
# include <arpa/inet.h>
# include <sys/select.h>
# include <time.h>
# include <netinet/ip_icmp.h>
# include <signal.h>
# include <math.h>
# include <errno.h>

//link list
# include "libft.h"

# define TIMEOUTSEC  1
# define TIMEOUTUSEC 0
# define TIMETOLIVE 64
# define PAYLOADSTRING "Hi, this is Ping echo request!Abcde"
# define PAYLOADLENGTH 36
# define EPSILON 1e-6

struct packet_timer {
    struct timeval  send_time;
    struct timeval  recv_time;
    int             seq_nbr;
    int             error;
};

struct packet {
    int                     fd;
    fd_set                  read;
    fd_set                  tmp_read;
    char                    send_buffer[64];
    char                    recv_buffer[1024];
    struct sockaddr_in      packet_address;
    char                    *host_ip_address;
    int                     sequence_number;
};


struct icmp_header {
    uint8_t type;        // ICMP type
    uint8_t code;        // ICMP code
    uint16_t checksum;   // Checksum
    uint16_t id;         // Identifier
    uint16_t sequence;   // Sequence number
};

//global variable
struct icmp_and_packet_timer
{
    struct icmp_header  *icmp;
    t_list              *timer_list;
    int                 socket_fd;
    char                *resolved_address;
    char                address[100];
    int                 nbr_of_packet_sent;
    int                 nbr_of_packet_lost;
};

extern struct icmp_and_packet_timer *timer_icmp;

//extern t_list				*timer_list;


//ft_split.c
char	    **ft_split(char const *s, char c);

//ft_utils.c
int		                ft_get_random_id();
uint16_t                compute_checksum(void *data, int len);
struct packet_timer     ft_initialize_timer(int seq_nbr);
void                    ft_set_recv_time(int seq_nbr, int len);

//ft_check_and_resolve_addr.c
int         ft_check_is_valid_ip(char *addr);
int         ft_check_and_resolve_addr(char **destination_addr);

//ft_check_for_error.c
int         ft_check_for_error(int ac, char **av, int *op);

//ft_check_for_help.c (-? option)
int         ft_check_for_help(int ac, char **av);

//ft_get_destination_address.c
int         ft_strlen(char *str);
int         ft_strcmp(char *s1, char *s2);
void        ft_free_destination(char **destination);
void        ft_free_new_destination(char **new_destination, int len);
char        **ft_add_address(char **destination_addr, char *addr, int word_nbr);
int         ft_count_addr(int ac, char **av);
char        **ft_get_destination_address(int ac, char **av);

//ft_get_host_ip_address.c
char        *ft_get_host_ip_address(char **destination);

//ft_initialize_socket.c
int         ft_initialize_socket(struct packet *pack, char **destination_addr);

//ft_check_parameters.c
int         ft_check_parameters(int ac, char **av, int *v_option, char ***destination_addr);

//ft_initialize_icmp_header.c
void        ft_initialize_icmp_header(struct icmp_header **icmp, struct packet *pack, int rand);
// struct packet  ft_initialize_icmp_header(struct icmp_header **icmp, struct packet pack);

//ft_signal_handler.c
void        ft_signal_handler(int a);
double      ft_get_packet_milisec(int seq_nbr);
double ft_get_stddev(int nbr_of_packet, double avg);

//ft_print_ip_icmp_header.c
void    ft_print_ip_header(struct iphdr *ip_header);
void    ft_print_icmp_header(struct icmp_header *icmp_header, int size);
void    ft_print_recv_packet(struct iphdr *ip_header, int size, struct icmp_header *icmp_header);
void    ft_print_source_hostname(uint32_t ip_address);

//ft_print_icmp_message.c icmp error types and codes
void    ft_print_icmp_message(int type, int code);

#endif