//#include "analyseip.h"
#include "checksum.h"
#include "lookuproute.h"
#include "arpfind.h"
#include "sendetherip.h"
#include "recvroute.h"
#include <pthread.h>


#define IP_HEADER_LEN sizeof(struct ip)
#define ETHER_HEADER_LEN sizeof(struct ether_header)

//int analyseIP(struct ip *ip)
//{
//	unsigned char* p = (unsigned char*)&ip->ip_src.s_addr;
//	printf("Source IP\t: %u.%u.%u.%u\n", p[0], p[1], p[2], p[3]);
//	p = (unsigned char*)&ip->ip_dst.s_addr;
//	printf("Destination IP\t: %u.%u.%u.%u\n", p[0], p[1], p[2], p[3]);
//	return 1;
//}
//接收路由信息的线程
void *thr_fn(void *arg)
{
	printf("thr_fn\n");
	int st=0;
	struct selfroute *selfrt; 
	selfrt = (struct selfroute*)malloc(sizeof(struct selfroute));
	memset(selfrt,0,sizeof(struct selfroute));

	//get if.name
	struct if_nameindex *head, *ifni;
	ifni = if_nameindex();
  	head = ifni;
	char *ifname;

	int sock_fd;
	struct sockaddr_in server_addr;
	sock_fd = socket(AF_INET, SOCK_STREAM, 0);
	bzero(&server_addr, sizeof(struct sockaddr_in));
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	server_addr.sin_port = htons(800);
	bind(sock_fd, (struct sockaddr *)(&server_addr), sizeof(struct sockaddr));
	listen(sock_fd, 5);
	while (1) {
		//printf("here1\n");
		int conn_fd = accept(sock_fd, (struct sockaddr *)NULL, NULL);
		//printf("here2\n");
		int ret = recv(conn_fd, selfrt, sizeof(struct selfroute), 0);
		printf("ret:%d\n", ret);
		if (ret > 0)
		{
			st = 1;
		}
		// do something with selfrt
		if (st == 1)
		{
			printf("cmdnum:%d\n", selfrt->cmdnum);
			if (selfrt->cmdnum == 24)
			{
				while (ifni->if_index != 0) {
					if (ifni->if_index == selfrt->ifindex)
					{
						printf("if_name is %s\n", ifni->if_name);
						ifname = ifni->if_name;
						break;
					}
					ifni++;
				}

				{
					//插入到路由表里


					printf("ifname:%s\n", ifname);
					printf("selfrt->prefix.s_addr:%d\n", selfrt->prefix.s_addr);
					printf("selfrt->nexthop.s_addr:%d\n", selfrt->nexthop.s_addr);
					printf("selfrt->ifindex:%d\n", selfrt->ifindex);
					insert_route(selfrt->prefix.s_addr, (unsigned int)selfrt->prefixlen, ifname, selfrt->ifindex, selfrt->nexthop.s_addr);
				}
			}
			else if (selfrt->cmdnum == 25)
			{
				//从路由表里删除路由
				delete_route(selfrt->prefix, (unsigned int)selfrt->prefixlen);
			}
		}
		close(conn_fd);
	}
	// add-24 del-25
	//while(1)
	//{
	//	printf("here0\n");
	//	st=static_route_get(selfrt);
	//	printf("after static_route_get()\n");
	//	if(st == 1)
	//	{
	//		printf("cmdnum:%d\n", selfrt->cmdnum);
	//		if(selfrt->cmdnum == 24)
	//		{
	//			while(ifni->if_index != 0) {
	//				if(ifni->if_index==selfrt->ifindex)
	//				{
	//					printf("if_name is %s\n",ifni->if_name);
	//						ifname= ifni->if_name;
	//					break;
	//				}
	//				ifni++;
	//			}

	//			{
	//				//插入到路由表里
	//				insert_route(selfrt->prefix.s_addr, (unsigned int)selfrt->prefixlen, selfrt->ifname, selfrt->ifindex, selfrt->nexthop.s_addr);
	//			}
	//		}
	//		else if(selfrt->cmdnum == 25)
	//		{
	//			//从路由表里删除路由
	//			delete_route(selfrt->prefix, (unsigned int)selfrt->prefixlen);
	//		}
	//	}

	//}

}

int main()
{
	char skbuf[1514];
	char data[1480];
	int recvfd, datalen;
	int recvlen;
	struct ip *ip_recvpkt;
	pthread_t tid;
	ip_recvpkt = (struct ip*)malloc(sizeof(struct ip));

	//创建raw socket套接字
	if ((recvfd = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_IP))) == -1)
	{
		printf("recvfd() error\n");
		return -1;
	}

	//路由表初始化
	route_table = (struct route*)malloc(sizeof(struct route));

	if (route_table == NULL)
	{
		printf("malloc error!!\n");
		return -1;
	}
	memset(route_table, 0, sizeof(struct route));
	//printf("before insert\n");

	{

	//-------------------------------------------------------------------------------------------------------
	////创建线程去接收路由信息
	}
	int pd;
	pd = pthread_create(&tid,NULL,thr_fn,NULL);
	printf("%d\n", pd);
	while(1)
	{
		//接收ip数据包模块
		//printf("start to recieve ip package\n");
		recvlen=recv(recvfd,skbuf,sizeof(skbuf),0);
		if(recvlen>0)
		{
			//printf("recieve successfully\n");
			ip_recvpkt = (struct ip *)(skbuf+ETHER_HEADER_LEN);
			struct ether_header *eth_header = (struct ether_header *)skbuf;
			//192.168.1.10是测试服务器的IP，现在测试服务器IP是192.168.1.10到192.168.1.80.
			//使用不同的测试服务器要进行修改对应的IP。然后再编译。
			//192.168.6.2是测试时候ping的目的地址。与静态路由相对应。
 			//if(ip_recvpkt->ip_src.s_addr == inet_addr("192.168.1.10") && ip_recvpkt->ip_dst.s_addr == inet_addr("192.168.6.2") )
			//printf("-----------------------------***-------------------\n");
			//printf("ip_recvpkt->ip_src.s_addr:%d\n", ip_recvpkt->ip_src.s_addr);
			//printf("inet_addr('192.168.6.0'):%d\n", inet_addr("192.168.6.0"));
			//printf("ip_recvpkt->ip_dst.s_addr:%d\n", ip_recvpkt->ip_dst.s_addr);
			//printf("inet_addr('192.168.6.2')%d\n", inet_addr("192.168.6.2"));
			//printf("-----------------------------***-------------------\n");
			if (ip_recvpkt->ip_src.s_addr == inet_addr("192.168.1.1") && ip_recvpkt->ip_dst.s_addr == inet_addr("192.168.6.2"))
			{
				//分析打印ip数据包的源和目的ip地址
				//analyseIP(ip_recvpkt);
				int s;
				memset(data,0,1480);			
				for(s=0;s<1480;s++)
				{
					data[s]=skbuf[s+34];
				}
				printf("reciecve head \n");
				int iii = 0;
				for ( iii= 0; iii < 34; iii++) {
					printf("skbuf[%d]:%c ", iii,skbuf[iii]);
				}
				printf("recieve Source MAC Address: %02x:%02x:%02x:%02x:%02x:%02x\n",
					skbuf[0], skbuf[1], skbuf[2], skbuf[3], skbuf[4], skbuf[5]);
				printf("recieve Dest MAC Address: %02x:%02x:%02x:%02x:%02x:%02x\n",
					skbuf[6], skbuf[7], skbuf[8], skbuf[9], skbuf[10], skbuf[11]);
				//printf("before checksum\n");
					// 校验计算模块
					struct _iphdr *iphead;
					int c=0;

					iphead=(struct _iphdr *)malloc(sizeof(struct _iphdr));
					
					{
						//int i = 0;
						//printf("ip_old\n");
						//while (i < 1514) {
						//	printf(" %d", skbuf[i]);
						//	i++;
						//}
						//printf("\n");
						iphead = (struct ip *)(skbuf + ETHER_HEADER_LEN);
						unsigned short oldcksum = iphead->checksum;


						//printf("2iphead->ip_h_verlen:%d\n", iphead->h_verlen);
						//printf("2iphead->ip_tos:%d\n", iphead->tos);
						//printf("2iphead->ip_total_len:%u\n", iphead->total_len);
						//printf("2iphead->ip_ident:%u\n", iphead->ident);
						//printf("2iphead->ip_frag:%u\n", iphead->frag);
						//printf("2iphead->ip_ttl:%d\n", iphead->ttl);
						//printf("2iphead->ip_proto:%d\n", iphead->proto);
						//printf("old_checksum:%u\n", iphead->checksum);
						//printf("2iphead->ip_src.s_addr:%d\n", iphead->sourceIP);
						//printf("2iphead->ip_src.s_addr:%d\n", iphead->destIP);
						iphead->checksum = 0;
						//i = 0;
						//printf("ip_new\n");
						//while (i < 1514) {
						//	printf(" %d", skbuf[i]);
						//	i++;
						//}
						//printf("\n");
						c = check_sum((unsigned short *)iphead, 20, oldcksum);
						//printf("c:%d\n", c);
					//调用校验函数check_sum，成功返回1
					}
					if(c ==1)
					{
							printf("checksum is ok!!\n");
					}else
					{
							printf("checksum is error !!\n");
							return -1;
					}

					{
							iphead->ttl--;
							iphead->checksum = count_check_sum(iphead);
					//调用计算校验和函数count_check_sum，返回新的校验和 
					} 


					//查找路由表，获取下一跳ip地址和出接口模块
					struct nextaddr *nexthopinfo;
					nexthopinfo = (struct nextaddr *)malloc(sizeof(struct nextaddr));
					memset(nexthopinfo,0,sizeof(struct nextaddr));
					
					printf("iphead->sourceIP: %d \n", iphead->sourceIP);
					printf("iphead->destIP: %d \n", iphead->destIP);
					struct in_addr tmpdstip;//sss
					{
						tmpdstip.s_addr = iphead->destIP;
						
					//调用查找路由函数lookup_route，获取下一跳ip地址和出接口
						int i = lookup_route(tmpdstip, nexthopinfo);
						if (i == 0) {
							printf("error in look up !!\n");
							return -1;
						}
					}

					
					//arp find
					//struct arpmac *srcmac;
					//srcmac = (struct arpmac*)malloc(sizeof(struct arpmac));
					//memset(srcmac,0,sizeof(struct arpmac));
					{
						
					//调用arpGet获取下一跳的mac地址		
						char *p = (char *)malloc(16);
						inet_ntop(AF_INET, (void *)&nexthopinfo->ipv4addr.s_addr, p, INET_ADDRSTRLEN);

						printf("nexthopinfo->ifname: %c\n", nexthopinfo->ifname[0]);
						printf("nexthopinfo->ifname: %s\n", nexthopinfo->ifname);
						arpGet(eth_header->ether_dhost, nexthopinfo->ifname, p);
						printf("Destination MAC Address: %02x:%02x:%02x:%02x:%02x:%02x\n",
							eth_header->ether_dhost[0], eth_header->ether_dhost[1], eth_header->ether_dhost[2], eth_header->ether_dhost[3], eth_header->ether_dhost[4], eth_header->ether_dhost[5]);
						free(p);
					}

					//send ether icmp
					{
					//调用ip_transmit函数   填充数据包，通过原始套接字从查表得到的出接口(比如网卡2)将数据包发送出去
					//将获取到的下一跳接口信息存储到存储接口信息的结构体ifreq里，通过ioctl获取出接口的mac地址作为数据包的源mac地址
					//封装数据包：
					//<1>.根据获取到的信息填充以太网数据包头，以太网包头主要需要源mac地址、目的mac地址、以太网类型eth_header->ether_type = htons(ETHERTYPE_IP);
					//<2>.再填充ip数据包头，对其进行校验处理；
					//<3>.然后再填充接收到的ip数据包剩余数据部分，然后通过raw socket发送出去

						//struct arpmac *dstmac;
						//dstmac = (struct arpmac*)malloc(sizeof(struct arpmac));
						//memset(dstmac,0,sizeof(struct arpmac));	

						int sock;
						int res;
						struct ifreq ifr;
						sock = socket(AF_INET, SOCK_STREAM, 0);
						strcpy(ifr.ifr_name, nexthopinfo->ifname);
						if (ioctl(sock, SIOCGIFHWADDR, &ifr) == 0) {
							//dstmac->mac = malloc(6);
							//*(dstmac->mac) = *(ifr.ifr_hwaddr.sa_data);

							unsigned char mac[6];
							// the mac address can be directed copied to eth_header->ether_dhost
							memcpy(eth_header->ether_shost, (unsigned char *)ifr.ifr_hwaddr.sa_data, 6);
							printf("Source MAC Address: %02x:%02x:%02x:%02x:%02x:%02x\n",
								eth_header->ether_shost[0], eth_header->ether_shost[1], eth_header->ether_shost[2], eth_header->ether_shost[3], eth_header->ether_shost[4], eth_header->ether_shost[5]);
						}
						else {
							printf("check my mac error\n");
						}


						int fill = 0;
						//while (fill < 12)
						//{
						//	if (fill < 6)
						//	{
						//		skbuf[fill] = eth_header->ether_shost[fill];
						//	}
						//	else
						//	{
						//		skbuf[fill] = eth_header->ether_dhost[fill-6];
						//	}
						//	fill++;
						//}
						//fill = 0;
						unsigned char *ipp = (unsigned char *)iphead;
						while (fill < 20)
						{
							skbuf[fill + ETHER_HEADER_LEN] = ipp[fill];
							fill++;
						}

						printf("send head \n");
						printf("send Dest MAC Address: %02x:%02x:%02x:%02x:%02x:%02x\n",
							skbuf[0], skbuf[1], skbuf[2], skbuf[3], skbuf[4], skbuf[5]);
						printf("send Source MAC Address: %02x:%02x:%02x:%02x:%02x:%02x\n",
							skbuf[6], skbuf[7], skbuf[8], skbuf[9], skbuf[10], skbuf[11]);
						iii = 0;
						for (iii = 0; iii < 34; iii++) {
							printf("skbuf[%d]:%c ", iii, skbuf[iii]);
						}
						//void ip_transmit(struct ip *ip_packet,unsigned short checksum,char *name, unsigned char *nextmac,char *bufdata,int datalength)
						ip_transmit(ip_recvpkt, iphead->checksum, nexthopinfo->ifname, eth_header->ether_dhost,skbuf,1514);
						printf("after transmit \n");

					}
			}

		}
	}

	close(recvfd);	
	return 0;
}

