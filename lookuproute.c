#include "lookuproute.h"


int insert_route(unsigned long  ip4prefix,unsigned int prefixlen,char *ifname,unsigned int ifindex,unsigned long  nexthopaddr)
{
	printf("------------------------------\n");
	printf("insert_route()\n");
	struct route *p = (struct route*)malloc(sizeof(struct route));
	memset(p, 0, sizeof(struct route));
	p->nexthop = (struct nexthop*)malloc(sizeof(struct nexthop));
	memset(p->nexthop, 0, sizeof(struct nexthop));
	p->next = route_table->next;
	route_table->next = p;
	p->prefixlen = prefixlen;
	p->ip4prefix.s_addr = ip4prefix;
	
	p->nexthop->ifindex = ifindex;
	p->nexthop->ifname = ifname;
	p->nexthop->nexthopaddr.s_addr = nexthopaddr;

	return 1;
}

int lookup_route(struct in_addr dstaddr,struct nextaddr *nexthopinfo)
{
	printf("------------------------------\n");
	printf("lookup_route()\n");
	
	struct route *p = route_table->next;
	//check_node(p);
	//check_node(p->next);
	while (p != NULL) {
		if (p->ip4prefix.s_addr == dstaddr.s_addr) {
			check_node(p->next);
			printf("find\n");
			nexthopinfo->ifname = p->nexthop->ifname;
			nexthopinfo->prefixl = p->nexthop->ifindex;
			nexthopinfo->ipv4addr.s_addr = p->nexthop->nexthopaddr.s_addr;
			return 1;
		}
		else {
			p = p->next;
		}
	}
	printf("not find\n");
	return 0;
}

int delete_route(struct in_addr dstaddr,unsigned int prefixlen)
{
	printf("------------------------------\n");
	printf("delete_route()\n");
	struct route *p = route_table;
	
}

void check_route_table() {
	printf("------------------------------\n");
	printf("check_route_table()\n");
	struct route *p = route_table;
	while (p->next != NULL) {
		check_node(p->next);
		p = p->next;
	}
}

void check_node(struct route* p){
	printf("------------------------------\n");
	printf("check_node\n");
	if (p != NULL) {
		printf("not null\n");
		printf("%s\n", p->nexthop->ifname);
		printf("%u\n", p->ip4prefix.s_addr);
		//printf("p->prefixlen:");
		//printf(p->prefixlen);
		//printf("\n");
		//printf("p->ip4prefix.s_addr");
		//printf(p->ip4prefix.s_addr);
		//printf("\n");
		//printf("p->nexthop->ifindex");
		//printf(p->nexthop->ifindex);
		//printf("\n");
		//printf("p->nexthop->ifname");
		//printf(p->nexthop->ifname);
		//printf("\n");
		//printf("p->nexthop->nexthopaddr.in_addr_t");
		//printf(p->nexthop->nexthopaddr.s_addr);
		//printf("\n");
	}
	else {
		printf("node is null");
	}
}

