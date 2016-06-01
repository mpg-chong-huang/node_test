int CBack;
char CBHR[2] = {"HR"};
char SystemNote[20] = {"SystemNote.txt"};
struct sockaddr_in ServerAddr,ClientAddr;	//struct sockaddr_in是internet环境下套接字的地址形式
											//自启动负荷节点时所用名字
DWORD WINAPI ThreadCBack( LPVOID lpParam )//多线程
{
	int accept_socket, AddrLen = 16, n =3;			//socket句柄.
	while(n--)
	{
		if ((accept_socket=accept(CBack,(struct sockaddr *)&ClientAddr,&AddrLen))==-1)
		{
			printf("\t\t再连接主总控失败!\n",n);
		}
		else
		{	printf("\t\t再连接主总控成功!\n\t\t此时主总控作为备用使用!\n",n);break;	}
	}
	while(1)
	{
		if (send(accept_socket,CBHR,2,0)==-1)
		{
			printf("\t\t备用用总控节点关闭\n");
			break;
		}
		Sleep(10);
	}
	CreateThread( NULL, 0, ThreadCBack, NULL, 0, 0 );				//心跳多线程试图与在开启的主总控建立连接
}