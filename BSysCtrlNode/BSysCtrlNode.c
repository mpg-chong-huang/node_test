// SysCtrlNode.cpp : Defines the entry point for the console application.
//
#include "SysCtrlcode.h"

int main(int argc, char* argv[])
{	
	FILE *fp;
	char temp[31];
	int BSocket = 0;
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);//设置printf颜色
	printf("\t\t     _________\n"); 
	printf("\t\t    / ======= \\ \n");
	printf("\t\t   / __________\\ \n");
	printf("\t\t  | ___________ | \n");
	printf("\t\t  | |  Hello  | | \n");
	printf("\t\t  | |  -   -  | | <<负荷分担的可靠性备用总控>>\n");
	printf("\t\t  | |_________| |_______________ \n");
	printf("\t\t  \\_____________/                )\n"); 
	printf("\t\t  / =========== \\              /        \n");
	printf("\t\t / ::::::::::::: \\         =D-'\n");
	printf("\t\t(_________________)\n");
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 12);//设置printf颜色
	//初始化内存中的红黑树											
	//初始化ID栈
	//内存池初始化
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 11);//设置printf颜色
	BSocket = BSocketInit(20010);
	Connect2CtrlNode(BSocket);
	BClientHeart( BSocket );
	InitializeCriticalSection(&g_cs);							 //定义线程锁
	//使主总控作为备份用，应该是要用多线程的
	Sleep(100);
	GetSocketId(20000, 3);
	CreateThread( NULL, 0, ThreadCBack, NULL, 0, 0 );				//心跳多线程试图与在开启的主总控建立连接
	Sleep(100);
	InItiate(1, 6);	
	allinit();	//初始化负载节点以及副控
	Sleep(100);
	printf("\t\t请自行接入Test以及PLNode(1~5)程序:\n");
	GetSocketId(10000, 1);
	if (!openFile(SeverBack,&fp))						  //打开文件备份处理
	{//失败
		printf("\t\t打开失败\n");
	} 
	else
	{//成功
		//读文件，参数文件指针，字符指针地址
		printf("\t\t初始化数据中,请耐心等待................\n");
		while(!readFromFile(fp,temp))					//全部读取维护一棵内存红黑树
		{		
			
			temp[0] = '+';
			if( temp[1]!= '*' && temp[1] != '\0')
			{
				Addmodule(temp, 0);						//将从文件获取的每一红黑树子节点发送给存货的
			}
		}
		//关闭文件
		closeFile(fp);
		printf("\t\t初始化完成,系统已启动..................\n");
	}
	Sleep(100);
	accept_socket[0] = NewSocket(10000, SocketIdagain[0]);//测试节点无需多线程
	printfNum( );										//打印负载节点
	SysCtrlNodeSocket( );								//核心模块
	return 0;
}
