#include "Define.h"						
void JudgeSafe(int	n , char *ErrorCode )	//判断(Judge)此时负载均衡是否Safe
{
	int judge = 1,num;
	char * Num = &ErrorCode[2];
	num = atoi(Num);
	if ( num != P.N[n].Num )
	{	judge = 0;	}//从负荷节点返回的节点数值要和总控节点相对应
	if ( P.N[P.MIN].Num<=P.N[1].Num && P.N[P.MIN].Num<=P.N[2].Num && P.N[P.MIN].Num<=P.N[3].Num &&P.N[P.MIN].Num<=P.N[4].Num&& P.N[P.MIN].Num<=P.N[5].Num)
	{	judge = 0;	}//负载要均衡
	if ( 0 == judge )						//No Safe
	{	printf("Load Balancing  System Safe!\n");	}
	else									//Safe
	{	printf("Load Balancing  System No Safe!\n");}
}

void GetSocketId(u_short dport, int bool)					//获取监听socket句柄
{
	int SocketId;								//定义套接字变量.
	struct sockaddr SA1;						//struct sockaddr是通用的套接字地址
	LPCSTR ip;									//LPCSTR是Win32和VC++所使用的一种字符串数据类型
												//LPCSTR被定义成是一个指向以NULL（‘\0’）结尾的常量字符的指针。
	WSADATA wsaData;							//这个结构被用来存储 被WSAStartup函数调用后返回的 Windows Sockets数据
												//它包含Winsock.dll执行的数据。
	PHOSTENT hostinfo;							//主机信息数据结构变量,用于存放函数返回的主机参数.
	char name[255];								//用于存放主机名
	if ( WSAStartup( MAKEWORD( 2, 2 ), &wsaData ) == 0 )
	{ //调用WSAStartup函数是加载Winsock.dll库，如果WSAStartup()函数的返回值为0，说明加载成功
			if( gethostname ( name, sizeof(name)) == 0) 
				{   //下面获取主机的IP地址,以备程序后面使用. //判断是否成功的将本地主机名存放入由name参数指定的缓冲区中
					if((hostinfo = gethostbyname(name)) != NULL) //获取主机的信息.
						{ //如果获得主机名成功的话，调用inet_ntoa()函数取得IP地址
							ip= inet_ntoa (*(struct in_addr *)*hostinfo->h_addr_list);
						} //将网络字节顺序转换成本机字节顺序
				}
			//创建插口 socket.
			//AF_INET为TCP/IP协议簇V4版本 //SOCK_STREAM为字节流插口(stream socket),实为TCP协议. 
			//0 因为TYPE已经指明是TCP协议,所以协议这里用0,代表使用默认协议.
			if ((SocketId=socket(AF_INET,SOCK_STREAM,0))==-1)	
				{
					printf("创建SOCKET失败!!!\n");
					exit(1);	//创建SOCKET不成功,转到错误处理.
				} //创建SOCKET成功
			ServerAddr.sin_addr.S_un.S_addr= inet_addr(ip);  //inet_addr()返回的地址已经是网络字节格式，所以你无需再调用 函数htonl()。
			ServerAddr.sin_family=AF_INET;					//指定为TCP/IP地址类型
			memset(&ServerAddr.sin_zero,8,0);				//从键盘读本机（服务器）将使用的端口,并赋给地址结构变量.
			ServerAddr.sin_port=htons(dport);				//端口号,htons()主机格式转换为网络格式
				if (bind(SocketId,(struct sockaddr *)&ServerAddr,sizeof(SA1))==-1)
				{ //(struct sockaddr *)强制类型转换.
					printf("bind失败:%d\n",WSAGetLastError());
					exit(1);
				}
				if (listen(SocketId,5)==-1)			//监听
				{	
					printf("\n执行listen()函数失败!!!\n");
					exit(1);
				}
				if(1 == bool)
				{	SocketIdagain[dport%10000] = SocketId;}//bool为1时获取收发用监听socket
				else if(2 == bool)
				{	SocketHeart[dport%20010] = SocketId;}//bool为2时获取心跳机制用监听socket
				else if(3 == bool)
				{	CBack = SocketId;	}
	}
}
int NewSocket(u_short dport, int SocketId)	//获取连接时用socket句柄
{
		int accept_socket, AddrLen = 16, n, chcnt;			//socket句柄.
		char Test[20];
		n = dport%10000;
		if ((accept_socket=accept(SocketId,(struct sockaddr *)&ClientAddr,&AddrLen))==-1)
			{
				switch(dport)
				{
					case 10000:printf("\t\tTestProgress没有连接!\n");break;
					default:printf("\t\tPlNode%d没有连接!\n",n);break;
				}
				return 0;
			}
		else
		{
			P.N[n].bool = 0;
			switch(dport)
			{
				case 10000:printf("\t\tTestProgress连接成功!\n");break;
				default:
					{
						chcnt=recv(accept_socket,Test,20,0);
						Test[chcnt] = '\0';
						P.N[n].Num=atoi(Test); //连接成功负载节点时获得负载节点数量 
						printf("\t\tPlNode%d连接成功!\n",n);
					}
				break;
			}
		}
		return accept_socket;	
}
int NewHeartSocket(u_short dport, int SocketId)	//获取心跳用socket句柄
{
		int accept_socket, AddrLen = 16, n;			//socket句柄.
		if ((accept_socket=accept(SocketId,(struct sockaddr *)&ClientAddr,&AddrLen))==-1)
			{
				printf("\n心跳accept失败!!!\n");
				exit(1);
			}
		else
		{
			if(dport != 20010)
			{	
				printf("\t\tPlNode%d灾难拯救机制启动!\n",dport%20010);	
				write2Log(severnum[dport%20010],SystemNote,1);//1是启动
			}
			else
			{ 
				printf("\t\t副控节点启动\n");
				write2Log("副总控节点",SystemNote,1);//1是启动
			}
		}
		return accept_socket;	
}
DWORD WINAPI ThreadNewSocket( LPVOID lpParam )//多线程
{
	int n = *(int*)(lpParam), nZero = 0;
	accept_socket[n] = NewSocket(20000+n, SocketIdagain[n]);//需改造成多线程
	setsockopt(accept_socket[n],SOL_SOCKET,SO_SNDBUF,(char *)&nZero,sizeof(nZero));
	setsockopt(accept_socket[n],SOL_SOCKET,SO_RCVBUF,(char *)&nZero,sizeof(int));
	return 0;
}
int ThreadHeart( int n )//多线程
{
	char HR[2];
	while(1)
	{		
			if (recv(heart_socket[n],HR,2,0)==-1)	//使用accept函数返回的socket号!
			{
					P.N[n].bool = -1;
					P.N[n].Num = MAX;
					printf("\t\tPlNode%d已关闭!\n",n);
					EnterCriticalSection(&g_cs);
					inQueue(n);						//对关闭的负载节点编号入队列	
					LeaveCriticalSection(&g_cs);
					write2Log(severnum[n],SystemNote,0);//0是断开
					break;
			}
			Sleep(20);
	}
	InItiate(n, n+1);
	return 0;
}
DWORD WINAPI ThreadNewHeartSocket( LPVOID lpParam )//多线程
{
	int n = *(int*)(lpParam);
	heart_socket[n] = NewHeartSocket(20010+n, SocketHeart[n]);//需改造成多线程
	ThreadHeart(n);//多线程
	return 0;
}
int InItiate(int kn, int km )//初始化某个或某些负载节点
{
	int i;
	for(i =kn; i<km; i++ )
	{
		if(km-kn != 1)
		{
			if(i != 0)
			{
				GetSocketId(20000+i, 1);						//正常收发
			}
			GetSocketId(20010+i, 2);						//心跳
		}
		if(i != 0)
		{	
			CreateThread( NULL, 0, ThreadNewSocket, &i, 0, 0 );//收发多线程监听
		}
		Sleep(100);
		CreateThread( NULL, 0, ThreadNewHeartSocket, &i, 0, 0 );//心跳多线程监听
		Sleep(100);
	}
	return 0;
}	
