#include "PlNodeServer.h"
#include "Public.h"

//主函数
int main(int argc, char* argv[])
{
	int PlNode = 5;
	int dport = 20005;
	printf("                                《PlNode5》\n");

	//对内存池，红黑树，和ID回收栈的初始化
	allInit();
	//通信函数
	sockRun(dport,PlNode);
 
	return 0;
}


