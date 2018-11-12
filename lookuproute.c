#include "lookuproute.h"


int insert_route(unsigned long  ip4prefix,unsigned int prefixlen,char *ifname,unsigned int ifindex,unsigned long  nexthopaddr)
{
	printf("insert_route()");
			
}

int lookup_route(struct in_addr dstaddr,struct nextaddr *nexthopinfo)
{
	printf("lookup_route()");
}

int delete_route(struct in_addr dstaddr,unsigned int prefixlen)
{
	printf("delete_route()");
}

