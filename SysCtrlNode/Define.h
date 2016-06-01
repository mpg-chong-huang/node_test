#include <winsock2.h>
#include <stdio.h>
#include <io.h>
#include <string.h>
#include <sys/types.h>
#include "RBTree4Ctr.h"
#define MAX 9999999
typedef struct
{
	int bool;								//服务器是否开启
	int Num;								//服务器承载数量
}Server_Node;
											//负荷均衡算法及全局数据变量头文件
typedef struct								//服务器分配情况结构体
{
	Server_Node N[6];						//ABC服务器负载用户数量
	int Total;								//总负载用户数量
	int MIN;								//负载最轻的服务器。
} PIMES;
//............................................................
PIMES P={{{-1,MAX},{-1,MAX},{-1,MAX},{-1,MAX},{-1,MAX},{-1,MAX}},0,1};//初始化服务器分配
int SocketIdagain[6];						//监听的socket	
int SocketHeart[6];							//监听的heart socket										
struct sockaddr_in ServerAddr,ClientAddr;	//struct sockaddr_in是internet环境下套接字的地址形式
											//自启动负荷节点时所用名字
int accept_socket[6];						//对应的测试节点及负荷节点socket句柄.0是与副控心跳的socket
int heart_socket[6];						//对应的测试节点及负荷节点的heartsocket句柄.
DWORD WINAPI ThreadNewSocket( LPVOID lpParam );
char SeverBack[20] = {"PlNode.BU"};			//备份文件引用名
char HRZore[2] = {"HR"};					//副控心跳字符串
CRITICAL_SECTION g_cs;						//线程锁变量
int change_ID;                              //转发用的ID
char severnum[6][20] = {"XXX","PlNode1","PlNode2","PlNode3","PlNode4","PlNode5"};

void TensionMin()//计算负载压力最小的服务器编号
{
	int i = 0, j = 0, bool = 0;
	P.Total = 0;
	for( i =1 ; i < 6; i++ )
	{	P.Total += (P.N[i].bool != -1)?P.N[i].Num:0; }
	for( i = 1; i < 6; i++ )
	{
		bool = 0;
		for( j = 1; j < 6; j++)
		{
			if(P.N[i].Num > P.N[j].Num)
			{
				bool = 1;
			}
		}
		if( 0 == bool)
		{
			P.MIN = i;
			break;
		}
	}
}
void AgainConnect(u_short dport)			//重连接机制
{
	switch(dport)
	{
	case 10000:printf("\t\tTestProgress断开连接!\n\t\t请重新连接TestProgress程序:\n");break;
	default:printf("\t\tPlNode%d断开连接!\n\t\t请重新连接PlNode%d程序:\n",dport%10000,dport%10000);break;
	}
	accept_socket[dport%10000] = NewSocket(dport, SocketIdagain[dport%10000]);
}
int CheckSever(int n)						//检测节点n存在与否，不存在就向测试发送DS
{
	if(P.N[n].bool == -1)
	{
		if (send(accept_socket[0],"DS",2,0)==-1)//DS代表当前负载节点不存在
		{
			AgainConnect(10000 );			//重连接机制
			return 0;
		}
		return 0;
	}	
	return -1;
}
void AssignServer ()						//负载均衡
{
	P.N[P.MIN].Num++;
	P.Total++;
	TensionMin();
}
