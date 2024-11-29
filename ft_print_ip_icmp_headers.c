#include "ft_ping.h"

void ft_print_ip_header(struct iphdr *ip_header) {
    printf("IP Hdr Dump:\n ");

    // Print raw hex dump of the header (20 bytes for IPv4 standard header)
    uint8_t *header_bytes = (uint8_t *)ip_header;
    for (int i = 0; i < ip_header->ihl * 4; i++) 
    {
        printf("%02x", header_bytes[i]);
        if (i % 2)
            printf(" ");
        // if ((i + 1) % 16 == 0) // Line break after 16 bytes
            // printf("\n");
    }
    printf("\n");

    // Print parsed header fields
    printf("Vr HL TOS  Len   ID Flg  off TTL Pro  cks      Src	Dst	Data\n");
    printf("%d  %d   %02x %04x %04x   %x %04x  %02x  %02x %04x ", 
           ip_header->version, 
           ip_header->ihl,
           ip_header->tos, 
           ntohs(ip_header->tot_len), 
           ntohs(ip_header->id),
           (ntohs(ip_header->frag_off) >> 13) & 0x7, // Flags (top 3 bits of frag_off)
           ntohs(ip_header->frag_off) & 0x1FFF, // Offset (bottom 13 bits of frag_off)
           ip_header->ttl, 
           ip_header->protocol, 
           ntohs(ip_header->check));

    // Print source and destination addresses
    struct in_addr src_addr, dst_addr;
    src_addr.s_addr = ip_header->saddr;
    dst_addr.s_addr = ip_header->daddr;

    printf("%s  %s \n", inet_ntoa(src_addr), inet_ntoa(dst_addr));
}

void    ft_print_icmp_header(struct icmp_header *icmp_header, int size)
{
    printf("ICMP: type %d, code %d, size %d, id 0x%04x, seq 0x%04x\n", 
        icmp_header->type, icmp_header->code, size, icmp_header->id, ntohs(icmp_header->sequence));
}

void    ft_print_source_hostname(uint32_t ip_address) 
{
    char hostname[200];
    struct sockaddr_in sa;

    // Fill sockaddr_in structure
    sa.sin_family = AF_INET;
    sa.sin_addr.s_addr = ip_address;

    // Perform reverse DNS lookup
    if (getnameinfo((struct sockaddr *)&sa, sizeof(sa), hostname, 200, NULL, 0, 0) == 0) 
    {
        printf("%s", hostname);
    }
    else
    {
        printf("%s", inet_ntoa(sa.sin_addr));
    }
}

void    ft_print_recv_packet(struct iphdr *ip_header, int len, struct icmp_header *icmp_header)
{
    struct in_addr src_addr;
    src_addr.s_addr =  ip_header->saddr;
    printf("%ld bytes from ", (len - sizeof(struct iphdr)));
    ft_print_source_hostname(ip_header->saddr),
    printf(" (%s): ", inet_ntoa(src_addr));
    ft_print_icmp_message(icmp_header->type, icmp_header->code);
}