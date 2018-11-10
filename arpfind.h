#ifndef __ARP__
#define __ARP__
#include <stdio.h>   
#include <sys/types.h>  
#include <sys/socket.h>  
#include <netinet/in.h>  
#include <arpa/inet.h>  
#include <sys/ioctl.h>  
#include <net/if_arp.h>  
#include <string.h> 
#include <errno.h>
#include <unistd.h>
#include <netinet/if_ether.h>
#include <netpacket/packet.h>
#include <net/if_arp.h>
#include <net/if.h>
#include <netdb.h>
#include <ifaddrs.h>

struct arpmac
{
    unsigned char * mac;
    unsigned int index;
};


int arpGet(struct arpmac *srcmac,char *ifname, char *ipStr);

#endif 
