#include "arpfind.h"

int arpGet(unsigned char *srcmac,char *ifname, char *ipStr)  
{  
	printf("arpGet()");
	struct arpreq arp_req;
	struct sockaddr_in *sin;
	sin = (struct sockaddr_in *)&(arp_req.arp_pa);
	memset(&arp_req, 0, sizeof(struct arpreq));
	sin->sin_family = AF_INET;
	// 1.1.1.1 is the IP of next hop
	//inet_pton(AF_INET, "1.1.1.1", &(sin->sin_addr));
	// or you can
	sin->sin_addr.s_addr = inet_addr(ipStr);
	// eth1 is the name of interface of next hop
	strncpy(arp_req.arp_dev, ifname, IF_NAMESIZE - 1);
	int arp_fd = socket(AF_INET, SOCK_DGRAM, 0);

	printf("ifname:%s\n", ifname);
	printf("sin->sin_addr.s_addr: %u\n", sin->sin_addr.s_addr);

	printf("arp_fd: %d\n", arp_fd);
	printf("SIOCGARP: %d\n", SIOCGARP);
	
	int ret = ioctl(arp_fd, SIOCGARP, &arp_req);
	// be careful with the return value!
	if (ret < 0) {
		fprintf(stderr, "Get ARP entry failed for %s @%s : %s\n", ipStr, ifname, strerror(errno));
		close(arp_fd);
		return -1;
	}
	if (arp_req.arp_flags & ATF_COM) {
		// entry found
		/*unsigned char mac[6];*/
		// the mac address can be directed copied to eth_header->ether_dhost
		memcpy(srcmac, (unsigned char *)arp_req.arp_ha.sa_data, 6);
		printf("Destination MAC Address: %02x:%02x:%02x:%02x:%02x:%02x\n",
			srcmac[0], srcmac[1], srcmac[2], srcmac[3], srcmac[4], srcmac[5]);

		//srcmac->mac = (char*)malloc(6);
		//*(srcmac->mac) = *mac;
		//printf("Destination MAC Address inside: %02x:%02x:%02x:%02x:%02x:%02x\n",
		//	srcmac->mac[0], srcmac->mac[1], srcmac->mac[2], srcmac->mac[3], srcmac->mac[4], srcmac->mac[5]);
	}
	else {
		// entry not found
	}

	// always remember to close it
	close(arp_fd);
    return 0;  
}  
                                                                                                        
                                                                                                          
                                                                                                            
                                                                                                              
