#include "RBTree.h"

/************************************************************************/
//ID栈的定义
typedef struct rec_ID_stack{
    int ID;																	//回收的ID信息
    struct rec_ID_stack   *next;											//指针域
	int blank[8];															//保持地址对齐
}rec_ID_stack;

/************************************************************************/
//全局变量定义
int WinSocketClient;														//定义套接字变量.
int WinSocketClient1;												        //定义套接字变量.
struct sockaddr_in ServerAddr,ClientAddr;									//定义地址结构变量.				
LPCSTR ip;																	//本机IP地址
HANDLE consolehwnd;															//创建颜色句柄 
char resultMes[10] = "-1";													//返回给Ctr节点的信息
RBTree tree[62];															//树根节点
int sum_id=0;																//全局变量sum_id，表示已分配用户id总数
int exist_sum_id= 0;														//表示现存的id总数。
																			//因为被删除的用户结点ID不会回收，																			//所以sum_id总大于或等于exist_sum_id,同时表示用户数
rec_ID_stack * top;															//栈顶指针
int isConnect = 0;

/************************************************************************/
//函数定义
int socketInit();															//socket初始化函数
void socketGetData( int );													//接收数据处理函数
void getInfos( char * );													//解析从CtrPlNode传来的数据
DWORD WINAPI server( LPVOID );												//线程server,模拟为每个用户提供的服务
void push( int );															//被删除用户的ID被回收入栈
void pop();																	//栈顶的用户信息出栈
int check_SameName( int,char * );											//检测输入的用户名是否重名，是返回1，否返回0
void Insert( char *, int );													//逆序插入新用户信息
int Delete( char *, int );													//删除用户结点。被删除的用户结点ID不会回收
int RecUserCmd( char *,int );												//函数RecUserCmd实现新增用户信息或删除用户信息的功能
int getHash( char );														//得到哈希值
DWORD WINAPI HR( LPVOID );													//心跳线程函数

/************************************************************************/
//进栈
void push( int id)															//被删除用户的ID被回收入栈
{	   		  		       
    rec_ID_stack * r;
	r = (rec_ID_stack *) MemoryPool_alloc();
	r->ID=id;																//分配id给新用户		
    r->next = top->next; 
    top->next = r;  

 }

/************************************************************************/
//出栈
void pop()																	//栈顶的用户信息出栈
{
	rec_ID_stack * p;
	p = top ->next;
	top->next = top->next->next;	
	Memory_recover(p);
}

/************************************************************************/
//查询
int check_SameName(int Hash,char *input)									//检测输入的用户名是否重名，是返回1，否返回0
{	
	PRBTNode p = NULL;
	input = &input[1];
	p = searchTNode(&tree[Hash],input);

	if( 0 == strcmp(p->Name,input))
		return 1;
		
	return 0;
}

/************************************************************************/
//插入
void Insert( char * input, int Hash)										//逆序插入新用户信息
{	   		  		       
		int ID;
		PRBTNode p = NULL;
		if (top->next != NULL)
		{
            ID=top->next->ID;
			pop();		
		}
        else{
			ID = sum_id+1;													//分配id给新用户
			sum_id++;														//累加用户id个数 
		}

		p = insertRB(&tree[Hash],input,ID);
		exist_sum_id++;														//更新目前存在用户个数

		SetConsoleTextAttribute(consolehwnd, 10);							//设置字体为绿色
		printf("新增加用户:%s,ID:%d",p->Name,p->ID); 
		SetConsoleTextAttribute(consolehwnd, 11);							//设置字体为蓝色
		printf("\n本服务器用户的总数为：%d\n\n",exist_sum_id);
	
		//构建新线程为当前用户服务
		CreateThread( NULL, 0, server, NULL, 0, 0 );
		
 }

/************************************************************************/
//删除
int Delete(char * input, int Hash)											//删除用户结点。被删除的用户结点ID不会回收
{
		PRBTNode p = NULL;
		p = searchTNode(&tree[Hash],input);
		//查找成功
		if( 0 == strcmp(p->Name,input) )
		{	 
			 push(p->ID);

			 p = deleteRB(&tree[Hash],p);

			 SetConsoleTextAttribute(consolehwnd, 12);						//设置字体为红色
			 printf("删除用户:%s,ID:%d",p->Name,p->ID); 
		
			 Memory_recover(p);
			 exist_sum_id--;

			 SetConsoleTextAttribute(consolehwnd, 11);						//设置字体为蓝色
			 printf("\n本服务器用户的总数为：%d\n\n",exist_sum_id);
			 
			 return 0;														//返回0表示删除用户结点成功。
		}	
		
			return 1;														//返回1表示要删除的用户不存在
	
}


/************************************************************************/
//解析用户请求
int RecUserCmd(char * input,int PlNode)										//函数RecUserCmd实现新增用户信息或删除用户信息的功能
{
	char str1[20] = {'\0'};
	int  f;
	int Hash_Now = 0;
	char c;
	PRBTNode p=NULL;

		c = input[1];														//得到用户名的第一个字符
		Hash_Now = getHash(c);												//通过hash函数得到树根下标

		if ('+' == input[0])												//实现新增用户信息功能
		{					 
			 input = &input[1];
			 Insert(input, Hash_Now);										//调用函数Insert_S插入新用户结点	 
			 itoa(exist_sum_id,str1,10);
			 strcpy(resultMes,"1,");
			 strcat(resultMes,str1);
			 return 1;
		}
		else if('~' == input[0])											//删除用户
		{

			input = &input[1];
			f = Delete(input, Hash_Now);									//调用函数Delete_S删除结点用户
			
			if (1 == f)														//如果f值为1，则说明要删除的用户结点不存在
			{
				itoa(exist_sum_id,str1,10);
			    strcpy(resultMes,"3,");
				strcat(resultMes,str1);
				return 1;
			}
			 itoa(exist_sum_id,str1,10);
			 strcpy(resultMes,"1,");
			 strcat(resultMes,str1);
			return 1;
		}
		else if('#' == input[0])											//查询用户是否在该服务器上
		{
			if( 1 == check_SameName(Hash_Now,input) )
			{
				itoa(PlNode,str1,10);
				strcpy(resultMes,str1);
			}
			else
			{
				strcpy(resultMes,"0");
			}
			return 1;
		}
		else if('@' == input[0])											//查询服务器人数
		{
			itoa(exist_sum_id,resultMes,10);
			  return 1;
		}
		else if('_' == input[0])											//测试是否重名
		{
			f=check_SameName(Hash_Now,input);
			if (1 == f)
			{
				strcpy(resultMes,"2");
				SetConsoleTextAttribute(consolehwnd, 12);						//设置字体为红色
				return 1;
			}
			
			strcpy(resultMes,"1");
			return 1;
		}
		else if('R' == input[0])											//释放所有用户
		{	
			for(Hash_Now = 0;Hash_Now<62;Hash_Now++)						//遍历61棵树
			{
				while(tree[Hash_Now].root!=tree[Hash_Now].nil)				//若树头不是指向哨兵节点
				{
					printf("释放用户%s,ID:%d\n",tree[Hash_Now].root->Name,tree[Hash_Now].root->ID); 
					push(tree[Hash_Now].root->ID);							//回收ID
					exist_sum_id--;
					p = deleteRB(&tree[Hash_Now],tree[Hash_Now].root);
					Memory_recover(p);
				}
			}

			if(exist_sum_id == 0)
			{
				strcpy(resultMes,"1");                                      //释放成功
			}
			else
			{
				strcpy(resultMes,"2");										//释放失败
			}
			SetConsoleTextAttribute(consolehwnd, 11);						//设置字体为蓝色
			printf("已释放所有用户,当前用户数为：%d\n",exist_sum_id);
			return 1;
		}
		else if ('S' == input[0])
		{
			exit(1);
		}

	
		return 0;
}



/************************************************************************/
//socket初始化函数
int socketInit()
{
	char name[255];															//用于存放主机名
    PHOSTENT hostinfo;														//主机信息数据结构变量,用于存放函数返回的主机参数.
	WSADATA wsaData;														//用于WSAStartup函数返回值.
	  
	if ( WSAStartup( MAKEWORD( 2, 2 ), &wsaData ) == 0 )
	{
		//调用WSAStartup函数是加载Winsock.dll库，
		//如果WSAStartup()函数的返回值为0，说明加载成功
		//获取主机的IP地址,以备程序后面使用.
		if( gethostname ( name, sizeof(name)) == 0) 
		{
			//判断是否成功的将本地主机名存放入由name参数指定的缓冲区中
			if((hostinfo = gethostbyname(name)) != NULL) //获取主机的信息.
			{
				//如果获得主机名成功的话，调用inet_ntoa()函数取得IP地址

				ip= inet_ntoa (*(struct in_addr *)*hostinfo->h_addr_list);
				
			} 
		}
		
	//输出本机的IP地址在变量ip中. 
	//=============================================
    return 1; 
	}
	return 0;
}

void pre4Connect(int *Client,int port)
{
	u_short dport=port;
	consolehwnd = GetStdHandle(STD_OUTPUT_HANDLE);							//实例化句柄
	ClientAddr.sin_addr.S_un.S_addr= inet_addr(ip);							//设置本机的IP地址.
	//inet_addr()返回的地址已经是网络字节格式，所以你无需再调用 函数htonl()。
	
	ClientAddr.sin_family=AF_INET;											//指定为TCP/IP地址类型
	
	memset(&ClientAddr.sin_zero,8,0);	
	
	ServerAddr.sin_addr.S_un.S_addr= inet_addr(ip); 
	
	ServerAddr.sin_port=htons(dport);          								//端口号,htons()主机格式转换为网络格式
		
	ServerAddr.sin_family=AF_INET;
	
	memset(&ServerAddr.sin_zero,8,0);
	
	//创建插口 socket.
	if ((*Client=socket(AF_INET,SOCK_STREAM,0))==-1)	
		//AF_INET为TCP/IP协议簇V4版本
		//SOCK_STREAM为字节流插口(stream socket),实为TCP协议.
		//0 因为TYPE已经指明是TCP协议,所以协议这里用0,代表使用默认协议.
		{
		SetConsoleTextAttribute(consolehwnd, 12);							//设置字体为红色
		printf("创建SOCKET失败!!!\n");
		
		}
}

/************************************************************************/
//连接转发节点
void Connect2CtrlNode(int Client,int k)
{
	struct sockaddr SA1;
	int i = 1;
	while(1)
	{	Sleep(85);
		if (connect(Client,(struct sockaddr *)&ServerAddr,sizeof(SA1))==-1)
		{
			SetConsoleTextAttribute(consolehwnd, 12);						//设置字体为红色
			printf("\n连接总控节点失败!!!\n第");
				
			SetConsoleTextAttribute(consolehwnd, 11);						//设置字体为蓝色
			printf("%d",i);
			SetConsoleTextAttribute(consolehwnd, 12);						//设置字体为红色

			printf("次重连...\n\n");
			i ++;
			continue;
		}		
		i = 1;
			break;
	}
	SetConsoleTextAttribute(consolehwnd, 10);								//设置字体为绿色
	if(k)
	{
		printf("总控节点连接成功!\n\n");	
		itoa(exist_sum_id,resultMes,10);
		if (send(Client,resultMes,strlen(resultMes),0)==-1)					//发送数据,返回个服务器的信息
		{
			SetConsoleTextAttribute(consolehwnd, 12);						//设置字体为红色
			printf("发送失败!\n");
		}
	}
	else
	{
		printf("与总控节点建立心跳检测!\n\n",ip);
		isConnect =1;

	}
}

/************************************************************************/
//socket通信函数
void socketGetData(int PlNode)
{
	
    char InputStr[256];														//接收字节的缓冲区
    int sendchrcnt;															//接收到得字节流长度
	
	while(1)
	{
		
			if ((sendchrcnt=recv(WinSocketClient,InputStr,20,0))==-1)
			{
				continue;
			}
			else
			{
				InputStr[sendchrcnt]='\0';
				while (0 ==isConnect )
				{
					Sleep(1000);
				}
				if(	!RecUserCmd(InputStr,PlNode) )
				{
					printf("接收到的信令%s,系统无法解释!",InputStr);
					getchar();
				}
			}

       
			if (send(WinSocketClient,resultMes,strlen(resultMes),0)==-1)	//发送数据,返回服务器的信息
			{
				continue;
			}
			strcpy(resultMes,"-1");											//每次发送完毕重置
			
		Sleep(10);
	} 

		closesocket(WinSocketClient);
		WSACleanup( );														//卸载Winsock库，并释放所有资源
	}

/************************************************************************/
//提供服务
DWORD WINAPI server( LPVOID lpParam )
{
	//为用户提供某项服务
	 Sleep(2000);

			return 0;
}

/************************************************************************/
//hash函数                                                             
int getHash(char c)
{                                           
	if(c>='0' && c<='9' )                 //哈希值为0到9
	{
		return (c - '0');
	}
	else if(c>='A' && c<='Z')             //哈希值为10到35
	{
		return (c - 55);
	}
	else if(c>='a' && c<='z')             //哈希值为36到61
	{
		return (c - 61);
	}
		return -1;
}



/************************************************************************/
//心跳机制线程
DWORD WINAPI HR( LPVOID lpParam )
{
	int dport = *(int*)(lpParam);

	//连接前的准备
	pre4Connect(&WinSocketClient1,dport+10);
	//连接心跳端
	Connect2CtrlNode(WinSocketClient1,0);	

	while(1)
	{
		if (send(WinSocketClient1,"HR",2,0)==-1)
		{
			isConnect =0;
			SetConsoleTextAttribute(consolehwnd, 12);					//设置字体为红色
			printf("\n心跳包接收失败!!!\n");

			//收发端口重连
			pre4Connect(&WinSocketClient,dport);
			Connect2CtrlNode(WinSocketClient,1);	

			//心跳端口重连
			pre4Connect(&WinSocketClient1,dport+10);
			Connect2CtrlNode(WinSocketClient1,0);
			continue;
		}
		Sleep(20);
	}
	
	return 0;
}