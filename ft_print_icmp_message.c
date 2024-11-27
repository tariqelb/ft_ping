#include "ft_ping.h"

//#include <netinet/ip_icmp.h>  // For ICMP types and codes

//vim netinet/ip_icmp.h 
//man 7 icmp
// 0 Echo Reply
// 3 Destination Unreachable *
// 4 Source Quench *
// 5 Redirect
// 8 Echo Request
// B Time Exceeded *
// C Parameter Problem *
// D Timestamp Request
// E Timestamp Reply
// F Info Request
// G Info Reply
// H Address Mask Request
// I Address Mask Reply


void ft_print_icmp_message(int type, int code) 
{
    switch (type) 
    {
        case ICMP_ECHOREPLY:  // Type 0 - Echo Reply
            printf("Echo Reply\n");
            break;
        
        case ICMP_DEST_UNREACH:  // Type 3 - Destination Unreachable
            switch (code) 
            {
                case ICMP_NET_UNREACH:
                    printf("Destination Unreachable: Network Unreachable\n");
                    break;
                case ICMP_HOST_UNREACH:
                    printf("Destination Unreachable: Host Unreachable\n");
                    break;
                case ICMP_PROT_UNREACH:
                    printf("Destination Unreachable: Protocol Unreachable\n");
                    break;
                case ICMP_PORT_UNREACH:
                    printf("Destination Unreachable: Port Unreachable\n");
                    break;
                default:
                    printf("Destination Unreachable: Unknown Code\n");
            }
            break;

        case ICMP_SOURCE_QUENCH:  // Type 4 - Source Quench
            printf("Source Quench\n");
            break;

        case ICMP_REDIRECT:  // Type 5 - Redirect
            switch (code) {
                case ICMP_REDIR_NET:
                    printf("Redirect: Network\n");
                    break;
                case ICMP_REDIR_HOST:
                    printf("Redirect: Host\n");
                    break;
                case ICMP_REDIR_NETTOS:
                    printf("Redirect: TOS Network\n");
                    break;
                case ICMP_REDIR_HOSTTOS:
                    printf("Redirect: TOS Host\n");
                    break;
                default:
                    printf("Redirect: Unknown Code\n");
            }
            break;

        case ICMP_ECHO:  // Type 8 - Echo Request
            printf("Echo Request\n");
            break;

        case ICMP_TIME_EXCEEDED:  // Type B - Time Exceeded
            switch (code) {
                case ICMP_EXC_TTL:
                    printf("Time to live exceeded\n");
                    break;
                case ICMP_EXC_FRAGTIME:
                    printf("Time Exceeded: Fragment reassembly time exceeded\n");
                    break;
                default:
                    printf("Time Exceeded: Unknown Code\n");
            }
            break;

        case ICMP_PARAMETERPROB:  // Type C - Parameter Problem
            printf("Parameter Problem\n");
            break;

        case ICMP_TIMESTAMP:  // Type D - Timestamp Request
            printf("Timestamp Request\n");
            break;

        case ICMP_TIMESTAMPREPLY:  // Type E - Timestamp Reply
            printf("Timestamp Reply\n");
            break;

        case ICMP_INFO_REQUEST:  // Type F - Information Request
            printf("Information Request\n");
            break;

        case ICMP_INFO_REPLY:  // Type G - Information Reply
            printf("Information Reply\n");
            break;

        case ICMP_ADDRESS:  // Type H - Address Mask Request
            printf("Address Mask Request\n");
            break;

        case ICMP_ADDRESSREPLY:  // Type I - Address Mask Reply
            printf("Address Mask Reply\n");
            break;

        default:
            printf("Unknown ICMP Type: %d\n", type);
            break;
    }
}
