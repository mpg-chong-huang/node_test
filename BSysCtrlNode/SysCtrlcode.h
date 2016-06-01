#include "FourElect.h"
//各节点初始化启动
int SysCtrlNodeSocket( )			//总控模块函数socket通信核心
{
	char Name[20],Air[20],uName[20];//Air广播用
	int chcnt=0, n = 0, i =0;		//接收字符数，转移的负载编号
	FILE *fpp = NULL;				//负载文件读取指针
	PRBTNode p = NULL;
	while (1)	
	{ 
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 12);//设置printf颜色
				if ((chcnt=recv(accept_socket[0],Name,20,0))==-1)//使用accept函数返回的socket号!
				{
					AgainConnect(10000 );//重连接机制 
					continue;
				}
				Name[chcnt]='\0';		//设置字符串结束符.
				if(Name[0] !='R' && Name[0] != 'S')
				{
					while(QueHead != QueTail)
					{//打开文件，参数文件名，文件指针地址
						EnterCriticalSection(&g_cs);
						outQueue(&n);//得到队列中的负载节点的编号
						LeaveCriticalSection(&g_cs);
						printf("\t\t转移PINode%d数据中......................................\n",n);
						tempTree(n);//临时树存放被转移的数据
						initRB(&tree[n]);//初始化被转移的那个负载节点对应的树
						while(tree[0].root != tree[0].nil)					//若树头不是指向哨兵节点
						{			
							p = deleteRB(&tree[0],tree[0].root);//取出叶子节点
							strcpy(uName+1,p->Name);
							uName[0] = '+';
							change_ID = p->ID;
							Addmodule(uName,2);//将取出的数据按均衡分配转发出去
							Memory_recover(p);
						}
						initRB(&tree[0]);		   //临时树结束使命，初始化
						printf("\t\t转移PINode%d数据完成....................................\n",n);
						printfNum( );	    	//打印节点情况		
					}
				}
						switch(Name[0])			//操作逻辑判断；
						{
						case '+':				//添加用户
							{
								printf("\t\t添加用户操作:");
								if (CheckAllSever(1) == 0 )			//若一个负载节点都不存在
									exit(0);
								Addmodule(Name, 1);//加用户
								break;
							}
						case '~':				//删除用户
							{
								printf("\t\t删除用户操作:");
								if (CheckAllSever(1) == 0 )			//若一个负载节点都不存在
									exit(0);
								Omitemodule(Name);//删除用户	
								break;
							}
						case '#':				 //查询此用户在哪一个负荷节点上
							{	//因为查询操作一定是安全的
								printf("\t\t查询用户所在负荷节点操作:\n\t\tLoad Balancing  System Safe!\n");
								if (CheckAllSever(1) == 0 )			//若一个负载节点都不存在
									exit(0);
								refermodule(Name);
								break;
							}
						case '@':				//查负荷节点此时的节点数
							{
								printf("\t\tLoad Balancing  System Safe!\n");     //因为查询操作负载一定是安全的
								if (CheckAllSever(1) == 0 )			//若一个负载节点都不存在
									exit(0);
								Amodule(Name);  //查负荷节点此时的节点数
								break;
							}
						case 'R':				//查负荷节点此时的节点数
							{
								printf("\t\t注销操作\n\t\tLoad Balancing  System Safe!\n");     //因为注销操作负载一定是安全的
								if (CheckAllSever(1) == 0 )			//若一个负载节点都不存在
									exit(0);
								LogOff(Name);
								break;
							}
						case 'S':				//查负荷节点此时的节点数
							{
								printf("\t\t退出操作\n\t\tLoad Balancing  System Safe!\n");     //因为退出操作负载一定是安全的
								ShutDown(Name);
								if('5' == Name[1])
								{
									strcpy(CBHR, "SD");
									write2Log("负载均衡系统",SystemNote,0);//0是断开
									printf("\t\t1秒后 Shutdown！！！\n");
									Sleep(1000);
									exit(0);
								}
								break;
							}
						default: break;
						}	
					printfNum( );									//打印节点情况
			}
		WSACleanup( );//卸载Winsock库，并释放所有资源
	return 0;
}
int CheckAllSever( )						   //检测负载节点是否一个都不存在，不存在就向测试发送CS
{
	int i = 0;
	for ( i = 1; i< 6; i++)
	{
		if(P.N[i].bool == 0)
			break;
	}
	if(i == 6)									//负载一个不存在
	{
		allClean(SeverBack);					//清空全部数据包括备份文件
		printf("\n\t\t没有负载节点连接到系统,系统结束!!\n");
		if (send(accept_socket[0],"CS",20,0)==-1)//CS代表负载节点一个都不存在
		{
			AgainConnect(10000 );				//重连接机制
			return 0;							//0表示负载一个都不存在
		}
		printf("\t\t没有负载节点存在,系统退出!!\n");
		strcpy(CBHR, "SD");
		write2Log("负载均衡系统",SystemNote,0);//0是断开
		Sleep(2000);
		return 0;
	}	
	return -1;									//-1表示至少还有一个存在
}
int LogOff(char Name[20])				//R注销操作function
{
	int chcnt=0,ServerN=0;			     //接收字符数
	FILE *fp = NULL;
	ServerN = (int)(Name[1])-48;		//获取欲注销的负荷节点的编号
	if (CheckSever(ServerN) == 0)		//若当前L的负载节点不存在
		return 0;
	if (send(accept_socket[ServerN],Name,20,0)==-1)
	{
		if (send(accept_socket[0],"DS",20,0)==-1)//讲L到的结果反馈给测试节点
		{
			AgainConnect(10000 );				//重连接机制
			return -1;
		}
	}
	if ((chcnt=recv(accept_socket[ServerN],Name,20,0))==-1)	//使用accept函数返回的socket号!
	{
		if (send(accept_socket[0],"DS",20,0)==-1)//将R查询到的结果反馈给测试节点
		{
			AgainConnect(10000 );				//重连接机制
			return -1;
		}
	}
	P.N[ServerN].Num = 0;
	TensionMin( ); //负载均衡
	Name[chcnt]='\0';
	if (send(accept_socket[0],Name,20,0)==-1)//将R查询到的结果反馈给测试节点
	{
		AgainConnect(10000 );				//重连接机制
		return -1;
	}
	allClean(SeverBack);					//清空全部数据包括备份文件				
	return 0;
}

int ShutDown(char Name[20])				//S退出系统操作function
{
	int chcnt=0,ServerN=0;			     //接收字符数
	FILE *fp = NULL;
	ServerN = (int)(Name[1])-48;		//获取欲注销的负荷节点的编号
	if (CheckSever(ServerN) == 0)		//若当前L的负载节点不存在
		return 0;
	if (send(accept_socket[ServerN],Name,20,0)==-1)
	{
		if (send(accept_socket[0],"DS",20,0)==-1)//讲L到的结果反馈给测试节点
		{
			AgainConnect(10000 );				//重连接机制
			return -1;
		}
	}
	if ((chcnt=recv(accept_socket[ServerN],Name,20,0))==-1)	//使用accept函数返回的socket号!
	{
		if (send(accept_socket[0],"DS",20,0)==-1)//将R查询到的结果反馈给测试节点
		{
			AgainConnect(10000 );				//重连接机制
			return -1;
		}
	}
	P.N[ServerN].Num = 0;
	TensionMin( ); //负载均衡
	Name[chcnt]='\0';
	if (send(accept_socket[0],Name,20,0)==-1)//将R查询到的结果反馈给测试节点
	{
		AgainConnect(10000 );				//重连接机制
		return -1;
	}
	remove(SeverBack);//删除文件
	cleanRB();		  //删除内存数
	cleanStack();	  //删除栈
}

int printfNum( )         //将改变后的三个负荷节点的负荷数以及总的负荷节点总数打印
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 10);//设置printf颜色
	printf("\t\tPlNodeA:%d",(P.N[1].Num == MAX)?-1:P.N[1].Num);
	printf("\tPlNodeB:%d",(P.N[2].Num == MAX)?-1:P.N[2].Num);
	printf("\tPlNodeC:%d",(P.N[3].Num == MAX)?-1:P.N[3].Num);
	printf("\n\t\tPlNodeD:%d",(P.N[4].Num == MAX)?-1:P.N[4].Num);
	printf("\tPlNodeE:%d\n",(P.N[5].Num == MAX)?-1:P.N[5].Num);
	printf("\t\tAllPlNode:%d\n",P.Total);
	return 0;
}