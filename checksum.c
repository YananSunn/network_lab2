#include "checksum.h"

int check_sum(unsigned short *iphd, int len, unsigned short checksum)
{
	printf("check_sum()\n");
	//struct ip
	//printf("iphd:%d\n",&iphd);
	//printf("checksum:%u\n", checksum);
	if (checksum == count_check_sum(iphd))
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

unsigned short count_check_sum(unsigned short *iphd)
{
	printf("count_check_sum()\n");
	unsigned short size = 10;
	unsigned int cksum = 0;
	//每16位相加
	int i = 0;
	while(i<size)
	{
		cksum += iphd[i];
		//printf("iphd[%d]:%u\n", i,iphd[i]);
		//printf("cksum in while:%d\n", cksum);
		i++;
	}
	//最后的奇数字节
	/*if (size)
	{
		cksum += *(unsigned char*)iphd;
		printf("zw");
	}*/

	cksum = (cksum >> 16) + (cksum & 0xffff);	//将高16bit与低16bit相加
	//printf("cksum add with low 16 bits:%d\n", cksum);
	cksum += (cksum >> 16);	//将进位到高位的16bit与低16bit 再相加,确保高16位为0
	//printf("cksum:%d\n", cksum);
	return ~cksum; //最后将结果取反,得到checksum
}

