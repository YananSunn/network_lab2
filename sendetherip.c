#include "sendetherip.h"

struct ip *fill_ip_packet(struct ip *ip_packet,unsigned short checksum)
{
	printf("fill_ip_packet()");
}
 
/* ip transmit */
void ip_transmit(struct ip *ip_packet,unsigned short checksum,char *name, unsigned char *nextmac,char *bufdata,int datalength)
{
	printf("ip_transmit()\n");
	int sendfd = socket(AF_PACKET, SOCK_RAW, IPPROTO_RAW);
	struct sockaddr_ll sadr_ll;
	 // ´ÓÍø¿¨Ãû×Ö²é index
	sadr_ll.sll_ifindex = if_nametoindex(name); // index of next hop
	sadr_ll.sll_halen = ETH_ALEN;
	// mac_addr_to is the result of arp query
	memcpy(sadr_ll.sll_addr, nextmac, ETH_ALEN);
	// length should be equal to the length you receive from raw socket
	int result;
	if ((result = sendto(sendfd, bufdata, datalength, 0, (const struct sockaddr *)&sadr_ll, sizeof(struct sockaddr_ll))) == -1) {
		// send error
		printf("ip_transmit error()\n");
	}
	else {
		// send succeed
		printf("ip_transmit success()\n");
	}
	close(sendfd);
	return;
}

