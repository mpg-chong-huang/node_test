#include "Define.h"						
void JudgeSafe(int	n , char *ErrorCode )	//�ж�(Judge)��ʱ���ؾ����Ƿ�Safe
{
	int judge = 1,num;
	char * Num = &ErrorCode[2];
	num = atoi(Num);
	if ( num != P.N[n].Num )
	{	judge = 0;	}//�Ӹ��ɽڵ㷵�صĽڵ���ֵҪ���ܿؽڵ����Ӧ
	if ( P.N[P.MIN].Num<=P.N[1].Num && P.N[P.MIN].Num<=P.N[2].Num && P.N[P.MIN].Num<=P.N[3].Num &&P.N[P.MIN].Num<=P.N[4].Num&& P.N[P.MIN].Num<=P.N[5].Num)
	{	judge = 0;	}//����Ҫ����
	if ( 0 == judge )						//No Safe
	{	printf("Load Balancing  System Safe!\n");	}
	else									//Safe
	{	printf("Load Balancing  System No Safe!\n");}
}

void GetSocketId(u_short dport, int bool)					//��ȡ����socket���
{
	int SocketId;								//�����׽��ֱ���.
	struct sockaddr SA1;						//struct sockaddr��ͨ�õ��׽��ֵ�ַ
	LPCSTR ip;									//LPCSTR��Win32��VC++��ʹ�õ�һ���ַ�����������
												//LPCSTR���������һ��ָ����NULL����\0������β�ĳ����ַ���ָ�롣
	WSADATA wsaData;							//����ṹ�������洢 ��WSAStartup�������ú󷵻ص� Windows Sockets����
												//������Winsock.dllִ�е����ݡ�
	PHOSTENT hostinfo;							//������Ϣ���ݽṹ����,���ڴ�ź������ص���������.
	char name[255];								//���ڴ��������
	if ( WSAStartup( MAKEWORD( 2, 2 ), &wsaData ) == 0 )
	{ //����WSAStartup�����Ǽ���Winsock.dll�⣬���WSAStartup()�����ķ���ֵΪ0��˵�����سɹ�
			if( gethostname ( name, sizeof(name)) == 0) 
				{   //�����ȡ������IP��ַ,�Ա��������ʹ��. //�ж��Ƿ�ɹ��Ľ������������������name����ָ���Ļ�������
					if((hostinfo = gethostbyname(name)) != NULL) //��ȡ��������Ϣ.
						{ //�������������ɹ��Ļ�������inet_ntoa()����ȡ��IP��ַ
							ip= inet_ntoa (*(struct in_addr *)*hostinfo->h_addr_list);
						} //�������ֽ�˳��ת���ɱ����ֽ�˳��
				}
			//������� socket.
			//AF_INETΪTCP/IPЭ���V4�汾 //SOCK_STREAMΪ�ֽ������(stream socket),ʵΪTCPЭ��. 
			//0 ��ΪTYPE�Ѿ�ָ����TCPЭ��,����Э��������0,����ʹ��Ĭ��Э��.
			if ((SocketId=socket(AF_INET,SOCK_STREAM,0))==-1)	
				{
					printf("����SOCKETʧ��!!!\n");
					exit(1);	//����SOCKET���ɹ�,ת��������.
				} //����SOCKET�ɹ�
			ServerAddr.sin_addr.S_un.S_addr= inet_addr(ip);  //inet_addr()���صĵ�ַ�Ѿ��������ֽڸ�ʽ�������������ٵ��� ����htonl()��
			ServerAddr.sin_family=AF_INET;					//ָ��ΪTCP/IP��ַ����
			memset(&ServerAddr.sin_zero,8,0);				//�Ӽ��̶�����������������ʹ�õĶ˿�,��������ַ�ṹ����.
			ServerAddr.sin_port=htons(dport);				//�˿ں�,htons()������ʽת��Ϊ�����ʽ
				if (bind(SocketId,(struct sockaddr *)&ServerAddr,sizeof(SA1))==-1)
				{ //(struct sockaddr *)ǿ������ת��.
					printf("bindʧ��:%d\n",WSAGetLastError());
					exit(1);
				}
				if (listen(SocketId,5)==-1)			//����
				{	
					printf("\nִ��listen()����ʧ��!!!\n");
					exit(1);
				}
				if(1 == bool)
				{	SocketIdagain[dport%10000] = SocketId;}//boolΪ1ʱ��ȡ�շ��ü���socket
				else if(2 == bool)
				{	SocketHeart[dport%20010] = SocketId;}//boolΪ2ʱ��ȡ���������ü���socket
				else if(3 == bool)
				{	CBack = SocketId;	}
	}
}
int NewSocket(u_short dport, int SocketId)	//��ȡ����ʱ��socket���
{
		int accept_socket, AddrLen = 16, n, chcnt;			//socket���.
		char Test[20];
		n = dport%10000;
		if ((accept_socket=accept(SocketId,(struct sockaddr *)&ClientAddr,&AddrLen))==-1)
			{
				switch(dport)
				{
					case 10000:printf("\t\tTestProgressû������!\n");break;
					default:printf("\t\tPlNode%dû������!\n",n);break;
				}
				return 0;
			}
		else
		{
			P.N[n].bool = 0;
			switch(dport)
			{
				case 10000:printf("\t\tTestProgress���ӳɹ�!\n");break;
				default:
					{
						chcnt=recv(accept_socket,Test,20,0);
						Test[chcnt] = '\0';
						P.N[n].Num=atoi(Test); //���ӳɹ����ؽڵ�ʱ��ø��ؽڵ����� 
						printf("\t\tPlNode%d���ӳɹ�!\n",n);
					}
				break;
			}
		}
		return accept_socket;	
}
int NewHeartSocket(u_short dport, int SocketId)	//��ȡ������socket���
{
		int accept_socket, AddrLen = 16, n;			//socket���.
		if ((accept_socket=accept(SocketId,(struct sockaddr *)&ClientAddr,&AddrLen))==-1)
			{
				printf("\n����acceptʧ��!!!\n");
				exit(1);
			}
		else
		{
			if(dport != 20010)
			{	
				printf("\t\tPlNode%d�������Ȼ�������!\n",dport%20010);	
				write2Log(severnum[dport%20010],SystemNote,1);//1������
			}
			else
			{ 
				printf("\t\t���ؽڵ�����\n");
				write2Log("���ܿؽڵ�",SystemNote,1);//1������
			}
		}
		return accept_socket;	
}
DWORD WINAPI ThreadNewSocket( LPVOID lpParam )//���߳�
{
	int n = *(int*)(lpParam), nZero = 0;
	accept_socket[n] = NewSocket(20000+n, SocketIdagain[n]);//�����ɶ��߳�
	setsockopt(accept_socket[n],SOL_SOCKET,SO_SNDBUF,(char *)&nZero,sizeof(nZero));
	setsockopt(accept_socket[n],SOL_SOCKET,SO_RCVBUF,(char *)&nZero,sizeof(int));
	return 0;
}
int ThreadHeart( int n )//���߳�
{
	char HR[2];
	while(1)
	{		
			if (recv(heart_socket[n],HR,2,0)==-1)	//ʹ��accept�������ص�socket��!
			{
					P.N[n].bool = -1;
					P.N[n].Num = MAX;
					printf("\t\tPlNode%d�ѹر�!\n",n);
					EnterCriticalSection(&g_cs);
					inQueue(n);						//�Թرյĸ��ؽڵ��������	
					LeaveCriticalSection(&g_cs);
					write2Log(severnum[n],SystemNote,0);//0�ǶϿ�
					break;
			}
			Sleep(20);
	}
	InItiate(n, n+1);
	return 0;
}
DWORD WINAPI ThreadNewHeartSocket( LPVOID lpParam )//���߳�
{
	int n = *(int*)(lpParam);
	heart_socket[n] = NewHeartSocket(20010+n, SocketHeart[n]);//�����ɶ��߳�
	ThreadHeart(n);//���߳�
	return 0;
}
int InItiate(int kn, int km )//��ʼ��ĳ����ĳЩ���ؽڵ�
{
	int i;
	for(i =kn; i<km; i++ )
	{
		if(km-kn != 1)
		{
			if(i != 0)
			{
				GetSocketId(20000+i, 1);						//�����շ�
			}
			GetSocketId(20010+i, 2);						//����
		}
		if(i != 0)
		{	
			CreateThread( NULL, 0, ThreadNewSocket, &i, 0, 0 );//�շ����̼߳���
		}
		Sleep(100);
		CreateThread( NULL, 0, ThreadNewHeartSocket, &i, 0, 0 );//�������̼߳���
		Sleep(100);
	}
	return 0;
}	
