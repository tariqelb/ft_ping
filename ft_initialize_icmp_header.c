#include "ft_ping.h"


void ft_initialize_icmp_header(struct icmp_header **icmp, struct packet *pack, int rand) 
{
	// Generate random ID for the request
    //int rand = ft_get_random_id();  
    memset(*icmp, 0, sizeof(struct icmp_header));
	// Type 8 for Echo Request
	(*icmp)->type = 8;  
    (*icmp)->code = 0;
    (*icmp)->id = htons(rand);
    (*icmp)->sequence = htons(pack->sequence_number);
    (*icmp)->checksum = 0;

    // Total packet size (header + payload)
    int total_packet_size = sizeof(struct icmp_header) + PAYLOADLENGTH;

    // Reset the send buffer
    memset(&pack->send_buffer, 0, sizeof(pack->send_buffer));

    // Copy the ICMP header into the send buffer
    memcpy(pack->send_buffer, *icmp, sizeof(struct icmp_header));

    // Copy the payload into the send buffer after the header
    memcpy(pack->send_buffer + sizeof(struct icmp_header), PAYLOADSTRING, PAYLOADLENGTH);

    // Calculate the checksum for the entire packet (header + payload)
    (*icmp)->checksum = compute_checksum(pack->send_buffer, total_packet_size);

    // Copy the ICMP header back into the send buffer (with the correct checksum)
    memcpy(pack->send_buffer, *icmp, sizeof(struct icmp_header));
	
	// 	// Print the ICMP header details for debugging
	// struct icmp_header *temp = malloc(sizeof(struct icmp_header));
	// if (temp == NULL) {
	// 	printf("Memory allocation for temp failed\n");
	// 	return;
	// }
	// memcpy(temp, pack->send_buffer, sizeof(struct icmp_header));

	// printf("rand %d  %d\n", rand, ntohs(rand));
	// printf("ICMP Header Details:\n");
	// printf("Type: %d\n", temp->type);                 // ICMP type
	// printf("Code: %d\n", temp->code);                 // ICMP code
	// printf("ID: %d\n", ntohs(temp->id));              // ID (convert from network to host byte order)
	// printf("Sequence: %d\n", ntohs(temp->sequence));  // Sequence number (convert from network to host byte order)
	// printf("Checksum: 0x%x\n", ntohs(temp->checksum)); // Checksum (convert from network to host byte order)

	// // Free the allocated memory for temp
	// free(temp);
}
