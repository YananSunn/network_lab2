#include "sendetherip.h"

struct ip *fill_ip_packet(struct ip *ip_packet,unsigned short checksum)
{
	printf("fill_ip_packet()");
}
 
/* ip transmit */
void ip_transmit(struct ip *ip_packet,unsigned short checksum,char *name, unsigned char *nextmac,char *bufdata,int datalength)
{
	printf("ip_transmit()");
}

