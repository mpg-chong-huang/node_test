#include <winsock2.h>
#include <stdio.h>
#include <io.h>
#include <string.h>
#include <malloc.h> 
#include <stdlib.h>
#include <windows.h>

#ifndef _SOCKET_H
#define _SOCKET_H
#define FALSE 0
#define TRUE 1	
#define panduan1 2
#define panduan2 3
/*从代码维护和可读性等多方面考虑，编写代码应尽量避免使用太多全局变量，*/
/*考虑到本程序代码的实际情况，在本文件Socket.h中定义多个全局变量比较合适*/

/*************以下全局变量在本文件Socket.h中被使用***************/
int WinSocketClient;	                                            //定义套接字变量.
struct sockaddr_in ServerAddr,ClientAddr;	                        //定义地址结构变量.	
struct sockaddr SA1;			                                    //struct sockaddr是通用的套接字地址
LPCSTR ip;                                                          //LPCSTR是Win32和VC++所使用的一种字符串数据类型													            	//LPCSTR被定义成是一个指向以NULL（‘\0’）结尾的常量字符的指针。
WSADATA wsaData;	                                                //用于WSAStartup函数返回值.
char name[255];                                                     //用于存放主机名
PHOSTENT hostinfo;	                                                //主机信息数据结构变量,用于存放函数返回的主机参数.
static u_short dport=10000;                                       //dport表示测试进程与总控节点进程连接的端口号，与总控节点约定默认值为10000
int sendchrcnt;                                                     //表示接收数据的长度
/************************************************************************/

/*************以下全局变量在Socket.h、ShutAndRelAU.h、UserConnServ.h文件中被使用***************/
char InputStr[256];                                                 //主要用于存放接收总控节点进程的数据
char Message[20];                                                   //用于存放发送给服务器端即总控节点进程的信息。	
int IsConnet = FALSE;                                               //表示是否已经和总控节点建立连接，IsConnet的值初始为FALSE
int IsGetWinsock_dll=FALSE;
/**********************************************************************************************/

void CreateSocket()             //创建插口 socket
{ 
	if ((WinSocketClient=socket(AF_INET,SOCK_STREAM,0))==-1)	//AF_INET为TCP/IP协议簇V4版本
					                                            //SOCK_STREAM为字节流插口(stream socket),实为TCP协议.
					                                            //0 因为TYPE已经指明是TCP协议,所以协议这里用0,代表使用默认协议.
	{ 
		printf("创建SOCKET失败\n");//exit(1);	                    //创建SOCKET不成功
	}
	else             				                                //创建SOCKET成功
	{				
		ClientAddr.sin_addr.S_un.S_addr= inet_addr(ip);		        //设置测试进程的IP地址.
		ClientAddr.sin_family=AF_INET;		                        //指定为TCP/IP地址类型
		memset(&ClientAddr.sin_zero,8,0);		
		strcpy(InputStr,ip);                                        //将IP地址号复制到字符串数组InputStr中
		ServerAddr.sin_addr.S_un.S_addr= inet_addr(InputStr);       //设置总控节点进程的IP地址，这里测试进程和总控节点进程的IP地址一样
		ServerAddr.sin_port=htons(dport);	                        //端口号,htons()主机格式转换为网络格式
		ServerAddr.sin_family=AF_INET;                              //指定为TCP/IP地址类型                              
		memset(&ServerAddr.sin_zero,8,0);
		if (connect(WinSocketClient,(struct sockaddr *)&ServerAddr,sizeof(SA1))==-1)
		{					
			IsConnet=FALSE;  //printf("找不到总控节点服务器!\n");
		}
		else
		{
			   	                                                    //连接总控节点成功
			IsConnet=TRUE;         		                            //将IsConnet的值变为TRUE，表示已建立连接
		}
	}
}

void CreateConn()                                                   //函数实现功能：测试进程与总控节点进程建立连接
{
	if ( WSAStartup( MAKEWORD( 2, 2 ), &wsaData ) == 0 )            //调用WSAStartup函数是加载Winsock.dll库，
	{                                                               //如果WSAStartup()函数的返回值为0，说明加载成功		
		if( gethostname ( name, sizeof(name)) == 0)                	//下面获取主机的IP地址,以备程序后面使用.
		{                                                           //判断是否成功的将本地主机名存放入由name参数指定的缓冲区中
			if((hostinfo = gethostbyname(name)) != NULL)            //获取主机的信息.
			{				
				ip= inet_ntoa (*(struct in_addr *)*hostinfo->h_addr_list);  //如果获得主机名成功的话，调用inet_ntoa()函数取得IP地址
			} 
		}
		IsGetWinsock_dll=TRUE;
		CreateSocket();		
	}
}//end of CreateConn()

void ConnectToSysCtrNode()
{                                                //由IsConnet的值判断测试进程是否已与总控节点进程建立连接
	if(IsGetWinsock_dll==FALSE)
	{
		CreateConn();
	}
	else
	{
		CreateSocket();
	}
}

int  SendAndRecvMsg(char * Message)                   //函数实现功能：实现与总控节点的通信
{

	char Msg[20];
	int IsOk = FALSE;
	strcpy(Msg,Message);
	printf("发送数据");
     while(FALSE == IsOk)
	 {
		if(send(WinSocketClient,Message,strlen(Message),0)==-1)  //向总控节点进程发送数据	
		{
				 IsConnet=FALSE;
				while(FALSE==IsConnet)
				{
					 CreateSocket();
					 Sleep(100);
					 printf("。");
				}
								
		}
		else
		 IsOk=TRUE;
	 }
	 printf("完成\n");	
	 IsOk = FALSE;	 
	 printf("接收数据");
	 while(FALSE == IsOk)
	 {
		if( (sendchrcnt=recv(WinSocketClient,InputStr,20,0))==-1) //接收来自总控节点进程的数据，字符串数组InputStr存放接收数据
		{
				IsConnet=FALSE;				
				while(IsConnet==FALSE)  
				{
					 CreateSocket();
					 Sleep(100);
					  printf("。");
				}
				if (TRUE == IsConnet)
				{
					send(WinSocketClient,Message,strlen(Message),0);

				}
		}
		else
			IsOk=TRUE;
			
	 }
	 printf("完成\n");	
	   InputStr[sendchrcnt]='\0';
	return TRUE;
}//end of SendAndRecvMsg(char * Message)

int CheckMsgFromSysCtrNode()                 //函数实现功能：检测总控节点的恢复信息，根据信息做判断
{

	if (InputStr[0]=='C'&&InputStr[1]=='S')//所有负荷节点PlNode服务器都关闭了，所有用户都已经被释放(不能恢复)。
	{
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
		printf("没有负荷节点进程开启，不存在任何用户服务\n");
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 14);
		printf("没有负荷节点开启，本次测试到此结束，用户将全部丢失\n");	
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
		printf("测试进程将在5秒后关闭\n");
		Sleep(5000);				
			exit(0);
		return panduan1;
	}
	return panduan2;
}

#endif
