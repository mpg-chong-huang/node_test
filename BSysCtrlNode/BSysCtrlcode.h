#include "CBack.h"
//ȫ�ֱ�������
struct sockaddr_in ServerAddr,ClientAddr;									//�����ַ�ṹ����.				
LPCSTR ip;																	//����IP��ַ
int BSocketInit(int port)
{
	int BClientSocket = 0;
	u_short dport=port;
	char name[255];															//���ڴ��������
    PHOSTENT hostinfo;														//������Ϣ���ݽṹ����,���ڴ�ź������ص���������.
	WSADATA wsaData;														//����WSAStartup��������ֵ.
	if ( WSAStartup( MAKEWORD( 2, 2 ), &wsaData ) == 0 )
	{
		if( gethostname ( name, sizeof(name)) == 0) 
		{//�ж��Ƿ�ɹ��Ľ������������������name����ָ���Ļ�������
			if((hostinfo = gethostbyname(name)) != NULL) //��ȡ��������Ϣ.
			{//�������������ɹ��Ļ�������inet_ntoa()����ȡ��IP��ַ
				ip= inet_ntoa (*(struct in_addr *)*hostinfo->h_addr_list);
				
			} 
		} 
	}
	ClientAddr.sin_addr.S_un.S_addr= inet_addr(ip);							//���ñ�����IP��ַ.
	ClientAddr.sin_family=AF_INET;											//ָ��ΪTCP/IP��ַ����
	memset(&ClientAddr.sin_zero,8,0);	
	ServerAddr.sin_addr.S_un.S_addr= inet_addr(ip); 
	ServerAddr.sin_port=htons(dport);          								//�˿ں�,htons()������ʽת��Ϊ�����ʽ
	ServerAddr.sin_family=AF_INET;
	memset(&ServerAddr.sin_zero,8,0);
	if ((BClientSocket=socket(AF_INET,SOCK_STREAM,0))==-1)					//������� socket.
	{
		printf("����SOCKETʧ��!!!\n");
	}
	return BClientSocket;
}
//����ת���ڵ�
void Connect2CtrlNode(int Client)
{
	struct sockaddr SA1;
	int i = 1;
	while(1)
	{	
		if (connect(Client,(struct sockaddr *)&ServerAddr,sizeof(SA1))==-1)
		{
			printf("\n\t\t�������ܿؽڵ�ʧ��!!!\n\t\t��");
			printf("%d",i);
			printf("������...\n");
			i ++;
			continue;
		}
		else
		{	printf("\t\t�����ܿؽڵ�ɹ�\n");	}
		break;
	}
}
int BClientHeart( int BSocket )
{
	char HR[2];
	int chcnt = 0;
	while(1)
	{		
		if ((chcnt = recv(BSocket,HR,2,0))==-1)	//ʹ��accept�������ص�socket��!
			{
					printf("\t\t���ܿؽڵ��ѹر�!!!\n");
					printf("\t\t���ܿؽڵ�ӹܹ���!!!\n");
					write2Log("���ܿؽڵ�",SystemNote,0);
					break;
			}
		HR[chcnt] = '\0';
		if ( 'S' == HR[0] )
		{	
			printf("\t\t��ϵͳ�˳��������ر�\n");
			Sleep(1000);
			exit(0);
		}
			Sleep(10);
	}
	return 0;
}