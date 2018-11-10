#include "checksum.h"

int check_sum(unsigned short *iphd, int len, unsigned short checksum)
{
	printf("check_sum()");
	//struct ip
	
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
	printf("check_sum()");
	unsigned short size = 10;
	unsigned int cksum = 0;
	//每16位相加
	int i = 0;
	while(i<size)
	{
		cksum += iphd[i++];
	}
	//最后的奇数字节
	/*if (size)
	{
		cksum += *(unsigned char*)iphd;
		printf("zw");
	}*/
	cksum = (cksum >> 16) + (cksum & 0xffff);	//将高16bit与低16bit相加
	cksum += (cksum >> 16);	//将进位到高位的16bit与低16bit 再相加,确保高16位为0
	return ~cksum; //最后将结果取反,得到checksum
}

