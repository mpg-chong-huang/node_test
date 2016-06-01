#include "FunctionHead.h"
										//四种操作函数
int Addmodule(char Name[20], int bool)	//"+"加操作function/,1表示需要回馈给测试，0表示不需要
{
	char Airing[20],Test[20];			//Air广播用
	int chcnt=0, i=0,id4File = -1;;					//接收字符数
		strcpy(Airing, Name);
		Airing[0] = '_';
		if(2 != bool)
		{
				for (i=1; i<6; i++) 
				{
					if( 0 != bool)
					{
						if (CheckAllSever(1) == 0 )		//若一个负载节点都不存在
						exit(0);
					}
					if(0 == P.N[i].bool)			//此负载节点存在
					{
							if (send(accept_socket[i],Airing,20,0)==-1)
								{
									continue;
								}
							if ((chcnt=recv(accept_socket[i],Test,20,0))==-1)	//使用accept函数返回的socket号!
								{
									continue;
								}
						Test[chcnt]='\0';
						if ('2' == Test[0])//返回说明有重名
						{
							if(0 == bool)//参数-1是动态分配ID，不需要请填入ID
							write2RBTree(Name+1,i,sum_id-1);//插入内存
							break;//一但有重名就跳出循环
						}
					}
			}
		}
		else{i = 6;}
	if (6 == i)	//说明广播过三个负载节点后都无重名并找到正确可插入的负荷节点
	{
loop1:	if( 0 != bool)
		{
			if (CheckAllSever(1) == 0 )			//若一个负载节点都不存在
			exit(0);
		}
		TensionMin( ); //负载均衡
		i = P.MIN;//将此次操作要插入的负荷节点存起,用于JudgeSafe
		if (send(accept_socket[P.MIN],Name,20,0)==-1)
			{	
				goto loop1;
			}
		else 
		{
			if(0 == bool)//参数-1是动态分配ID，不需要请填入ID
				write2RBTree(Name+1,P.MIN,sum_id-1);//插入内存
			if(2 == bool)//转发用
			{
				changeTree(Name+1,P.MIN,change_ID);
			}
			if(1 == bool)
			{
				id4File = write2RBTree(Name+1,P.MIN,-1);//插入内存
				backup2File(Name+1,SeverBack,id4File);//添加进文件	
			}
		}
		if ((chcnt=recv(accept_socket[P.MIN],Name,20,0))==-1)	//使用accept函数返回的socket号!
			{
				strcpy(Name, "1");
			}
		AssignServer ();	//负载均衡
		Name[chcnt]='\0';	//设置字符串结束符.
		if(1 == bool)
		{
			if (send(accept_socket[0],Name,20,0)==-1)//讲新建用户成功的信息反馈给测试节点
			{
				AgainConnect(10000 );//重连接机制
				return -1;
			}
			JudgeSafe( i, Name );
		}
	}
	else  //因重名无法插入
	{
		if(1 == bool)
		{
			if (send(accept_socket[0],Test,20,0)==-1)
			{
				AgainConnect(10000 );//重连接机制
				return -1;
			}
		printf("  System Safe!\n");//此时添加不成功故系统一定是safe的
		}
	}
	TensionMin( ); //负载均衡
	return -1;
}
int Omitemodule(char Name[20])	//"~"删除操作function
{
	char Test[20];			//Air广播用
	int chcnt=0, i=0;					//接收字符数
	for(i=1; i<6; i++)//广播三个负载节点删除信息
	{
		if (CheckAllSever(1) == 0 )			//若一个负载节点都不存在
			exit(0);
		if(0 == P.N[i].bool)
		{
			if (send(accept_socket[i],Name,20,0)==-1)
			{
				continue;
			}
			if ((chcnt=recv(accept_socket[i],Test,20,0))==-1)	//使用accept函数返回的socket号!
			{
				continue;
			}
			Test[chcnt]='\0';					//设置字符串结束符.
			if('1' == Test[0])					//删除成功时
			{
				P.N[i].Num--;P.Total--;
				delNode(Name+1,SeverBack,i);
				TensionMin( ); //负载均衡
				JudgeSafe( i , Test );
				break;
			}
		}
	}
	if(6 == i)
	{
		printf("  Load Balancing  System Safe!\n");//此时删除不成功故系统一定是safe的
	}
	if (send(accept_socket[0],Test,20,0)==-1)//无论删除成功与否都会有一个信息反馈给测试节点
	{
		AgainConnect(10000 );				 //重连接机制
		return -1;							 //测试进程重连时返回值
	}
	TensionMin( ); //负载均衡
	return 0;								 //正常时返回值
}
int refermodule(char Name[20])				//"#"查询操作function
{
	char Test[20] = "0";			//Air广播用
	int chcnt=0, i=0;			//接收字符数
	for (i=1; i<6; i++) //广播三个负载节点查询信息
	{
		if (CheckAllSever(1) == 0 )			//若一个负载节点都不存在
			exit(0);
		if(0 == P.N[i].bool)
		{
			if (send(accept_socket[i],Name,20,0)==-1)
				{
					continue;
				}
			if ((chcnt=recv(accept_socket[i],Test,20,0))==-1)	//使用accept函数返回的socket号!
				{
					continue;
				}
			Test[chcnt]='\0';
			if ('0' != Test[0])//此时找到此用户在那一个负荷节点上
				{
					break;
				}
		}
	}
	if (send(accept_socket[0],Test,20,0)==-1)//无论查询是否成功都给测试节点一个信息
		{
			AgainConnect(10000 );		//重连接机制
			return -1;					//测试进程重连时返回值
		}
	return 0;							//正常时返回值
}
int Amodule(char Name[20])				//"@"查询操作function
{
	int chcnt=0,ServerN=0;				//接收字符数
	ServerN = (int)(Name[1])-48;		//获取欲查询的负荷节点的编号
	printf("\t\t查询(%d)负荷节点用户数操作:\n",ServerN);
	if (CheckSever(ServerN) == 0)		//若当前@的负载节点不存在
		return 0;
	if (send(accept_socket[ServerN],Name,20,0)==-1)
		{
			if (send(accept_socket[0],"DS",2,0)==-1)//讲@查询到的结果反馈给测试节点
			{
				AgainConnect(10000 );				//重连接机制
				return -1;
			}
		}
	if ((chcnt=recv(accept_socket[ServerN],Name,20,0))==-1)	//使用accept函数返回的socket号!
		{
			if (send(accept_socket[0],"DS",2,0)==-1)//讲@查询到的结果反馈给测试节点
			{
				AgainConnect(10000 );				//重连接机制
				return -1;
			}
		}
	Name[chcnt]='\0';
	if (send(accept_socket[0],Name,20,0)==-1)//讲@查询到的结果反馈给测试节点
		{
			AgainConnect(10000 );				//重连接机制
			return -1;
		}
	return 0;
}

