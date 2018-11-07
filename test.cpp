#include <iostream>
#include <stdlib.h>
using namespace std;
struct route
{
	int isRoute;
	int isFinishRoute;
    struct route *next[16];
    unsigned long ip4prefix;
	unsigned int prefixlen;
    struct nexthop *nexthop;
};

struct nexthop
{
   struct nexthop *next;
   char *ifname;
   unsigned int ifindex;//zlw ifindex2ifname()鑾峰彇鍑烘帴鍙?   // Nexthop address 
   unsigned long nexthopaddr;
};

struct nextaddr
{
   char *ifname;
   unsigned long ipv4addr;
   unsigned int prefixl;
};

struct route *route_table; 

void initial_route(struct route* p){
	p->isRoute = 0;
	p->isFinishRoute = 0;
    for(int i = 0; i < 16; i++){
    	p->next[i] = NULL;
	}
    p->ip4prefix = 0;
	p->prefixlen = 0;
    p->nexthop = NULL;
}

void initial_nexthop(struct nexthop* p){
   	p->next = NULL;
   	p->ifname = NULL;
   	p->ifindex = 0;
   	p->nexthopaddr = 0;
}

void printNode(struct route* p){
	cout<<"printNode"<<endl;
	if(p != NULL){
		cout<<"p->isRoute:"<<p->isRoute<<endl;
		if(p->isRoute == 1){
			cout<<"p->ip4prefix:"<<p->ip4prefix<<endl;
			cout<<"p->prefixlen:"<<p->prefixlen<<endl;
			
			if(p->nexthop != NULL){
				cout<<"p->nexthop->ifname:"<<p->nexthop->ifname<<endl;
				cout<<"p->nexthop->ifindex:"<<p->nexthop->ifindex<<endl;
				cout<<"ip->nexthop->nexthopaddr:"<<p->nexthop->nexthopaddr<<endl;
			}
			else{
				cout<<"not a finishing node"<<endl;
			}
		}
	}
	else{
		cout<<"the node is null"<<endl;
	}
}

int insert_route(unsigned long ip4prefix,unsigned int prefixlen,char *ifname,unsigned int ifindex,unsigned long nexthopaddr)
{
	printf("insert_route()\n");
	if(route_table == NULL){
		printf("route_table is null!\n");
	}
	
	int length = 0;
	int split[8] = {0};
	unsigned long tmp = ip4prefix;
	while(tmp > 0){
		tmp = tmp / 16;
		length++;
	}
//	cout<<"length:"<<length<<endl;
	
	tmp = ip4prefix;
	for(int i = length -1; i >= 0; i--){
		split[i] = tmp % 16;
		tmp = tmp / 16;
	}
	
//	for(int i = 0; i < length; i++){
//		cout<<split[i]<<endl;
//	}
	int single_node = 0;
	tmp = ip4prefix;
	struct route *p = route_table;
	
	for(int j = 0; j < length; j++)
	{
		single_node = split[j];
//		cout<<"single_node:"<<single_node<<endl;
		if (p->next[single_node] == NULL)        //如果不存在，则建立结点  
		{
			struct route *temp = (struct route*)malloc(sizeof(struct route));
			initial_route(temp);
			temp->isRoute = 1;
			temp->isFinishRoute = 0;
			p->next[single_node] = temp;
//			printNode(p);
			p = p->next[single_node];
			
		}
		else
		{
			p = p->next[single_node];
		}
	}
//	cout<<"here"<<endl;
	p->isFinishRoute = 1;
	p->ip4prefix = ip4prefix;
	p->prefixlen = prefixlen;
//	cout<<"here1"<<endl;
	p->nexthop = (struct nexthop*)malloc(sizeof(struct nexthop));
	initial_nexthop(p->nexthop);
	p->nexthop->ifindex = ifindex;
	p->nexthop->ifname = ifname;
	p->nexthop->nexthopaddr = nexthopaddr;
//	cout<<"here2"<<endl;
	
	cout<<"p->isRoute:"<<p->isRoute<<endl;
	cout<<"p->ip4prefix:"<<p->ip4prefix<<endl;
	cout<<"p->prefixlen:"<<p->prefixlen<<endl;
	cout<<"p->nexthop->ifname:"<<p->nexthop->ifname<<endl;
	cout<<"p->nexthop->ifindex:"<<p->nexthop->ifindex<<endl;
	cout<<"p->nexthop->nexthopaddr:"<<p->nexthop->nexthopaddr<<endl;
	
	cout<<"-------------------------------------------------"<<endl;
}

int lookup_route(unsigned long dstaddr,struct nextaddr *nexthopinfo)
{
	printf("lookup_route()\n");
}

int delete_route(unsigned long dstaddr,unsigned int prefixlen)
{
	printf("delete_route()\n");
}



void printTable(struct route* tmp){
	cout<<"printTable()"<<endl;
	while(1){
		if(tmp != NULL){
//			cout<<"here4"<<endl;
			for(int i = 0; i < 16; i++){
//				cout<<"here5"<<endl;
				if(tmp->next[i] != NULL){
					cout<<"i:"<<i<<endl;
					printNode(tmp->next[i]);
					printTable(tmp->next[i]);
				}
			}
			return;
		}
		else{
			printf("printTable() the note is null \n");
			return;
		}
	}
}

int main(){
	route_table=(struct route*)malloc(sizeof(struct route));
	initial_route(route_table);
	insert_route(0x305a8c0,24,"eth1",4,0x204a8c0);
	insert_route(0x325a8c0,24,"eth2",4,0x204a8c0);
	insert_route(0x625a8c0,24,"eth3",4,0x204a8c0);
	printTable(route_table);
	return 0;
} 
