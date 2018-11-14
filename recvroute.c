#include "recvroute.h"
#include <stdio.h>




int static_route_get(struct selfroute *selfrt)
{
	printf("static_route_get()\n");
	int sock_fd;
	struct sockaddr_in server_addr;
	sock_fd = socket(AF_INET, SOCK_STREAM, 0);
	bzero(&server_addr, sizeof(struct sockaddr_in));
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	server_addr.sin_port = htons(800);
	bind(sock_fd, (struct sockaddr *)(&server_addr), sizeof(struct sockaddr));
	listen(sock_fd, 5);
	printf("here1\n");
	//while (1) {
		int conn_fd = accept(sock_fd, (struct sockaddr *)NULL, NULL);
		printf("conn_fd:%d\n", conn_fd);
		printf("here2\n");
		int ret = recv(conn_fd, selfrt, sizeof(struct selfroute), 0);
		printf("ret:%d\n",ret);
		// do something with selfrt
		close(conn_fd);
		printf("here3\n");
		return 1;
	//}

}
