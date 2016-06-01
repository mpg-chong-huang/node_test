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
/*�Ӵ���ά���Ϳɶ��Եȶ෽�濼�ǣ���д����Ӧ��������ʹ��̫��ȫ�ֱ�����*/
/*���ǵ�����������ʵ��������ڱ��ļ�Socket.h�ж�����ȫ�ֱ����ȽϺ���*/

/*************����ȫ�ֱ����ڱ��ļ�Socket.h�б�ʹ��***************/
int WinSocketClient;	                                            //�����׽��ֱ���.
struct sockaddr_in ServerAddr,ClientAddr;	                        //�����ַ�ṹ����.	
struct sockaddr SA1;			                                    //struct sockaddr��ͨ�õ��׽��ֵ�ַ
LPCSTR ip;                                                          //LPCSTR��Win32��VC++��ʹ�õ�һ���ַ�����������													            	//LPCSTR���������һ��ָ����NULL����\0������β�ĳ����ַ���ָ�롣
WSADATA wsaData;	                                                //����WSAStartup��������ֵ.
char name[255];                                                     //���ڴ��������
PHOSTENT hostinfo;	                                                //������Ϣ���ݽṹ����,���ڴ�ź������ص���������.
static u_short dport=10000;                                       //dport��ʾ���Խ������ܿؽڵ�������ӵĶ˿ںţ����ܿؽڵ�Լ��Ĭ��ֵΪ10000
int sendchrcnt;                                                     //��ʾ�������ݵĳ���
/************************************************************************/

/*************����ȫ�ֱ�����Socket.h��ShutAndRelAU.h��UserConnServ.h�ļ��б�ʹ��***************/
char InputStr[256];                                                 //��Ҫ���ڴ�Ž����ܿؽڵ���̵�����
char Message[20];                                                   //���ڴ�ŷ��͸��������˼��ܿؽڵ���̵���Ϣ��	
int IsConnet = FALSE;                                               //��ʾ�Ƿ��Ѿ����ܿؽڵ㽨�����ӣ�IsConnet��ֵ��ʼΪFALSE
int IsGetWinsock_dll=FALSE;
/**********************************************************************************************/

void CreateSocket()             //������� socket
{ 
	if ((WinSocketClient=socket(AF_INET,SOCK_STREAM,0))==-1)	//AF_INETΪTCP/IPЭ���V4�汾
					                                            //SOCK_STREAMΪ�ֽ������(stream socket),ʵΪTCPЭ��.
					                                            //0 ��ΪTYPE�Ѿ�ָ����TCPЭ��,����Э��������0,����ʹ��Ĭ��Э��.
	{ 
		printf("����SOCKETʧ��\n");//exit(1);	                    //����SOCKET���ɹ�
	}
	else             				                                //����SOCKET�ɹ�
	{				
		ClientAddr.sin_addr.S_un.S_addr= inet_addr(ip);		        //���ò��Խ��̵�IP��ַ.
		ClientAddr.sin_family=AF_INET;		                        //ָ��ΪTCP/IP��ַ����
		memset(&ClientAddr.sin_zero,8,0);		
		strcpy(InputStr,ip);                                        //��IP��ַ�Ÿ��Ƶ��ַ�������InputStr��
		ServerAddr.sin_addr.S_un.S_addr= inet_addr(InputStr);       //�����ܿؽڵ���̵�IP��ַ��������Խ��̺��ܿؽڵ���̵�IP��ַһ��
		ServerAddr.sin_port=htons(dport);	                        //�˿ں�,htons()������ʽת��Ϊ�����ʽ
		ServerAddr.sin_family=AF_INET;                              //ָ��ΪTCP/IP��ַ����                              
		memset(&ServerAddr.sin_zero,8,0);
		if (connect(WinSocketClient,(struct sockaddr *)&ServerAddr,sizeof(SA1))==-1)
		{					
			IsConnet=FALSE;  //printf("�Ҳ����ܿؽڵ������!\n");
		}
		else
		{
			   	                                                    //�����ܿؽڵ�ɹ�
			IsConnet=TRUE;         		                            //��IsConnet��ֵ��ΪTRUE����ʾ�ѽ�������
		}
	}
}

void CreateConn()                                                   //����ʵ�ֹ��ܣ����Խ������ܿؽڵ���̽�������
{
	if ( WSAStartup( MAKEWORD( 2, 2 ), &wsaData ) == 0 )            //����WSAStartup�����Ǽ���Winsock.dll�⣬
	{                                                               //���WSAStartup()�����ķ���ֵΪ0��˵�����سɹ�		
		if( gethostname ( name, sizeof(name)) == 0)                	//�����ȡ������IP��ַ,�Ա��������ʹ��.
		{                                                           //�ж��Ƿ�ɹ��Ľ������������������name����ָ���Ļ�������
			if((hostinfo = gethostbyname(name)) != NULL)            //��ȡ��������Ϣ.
			{				
				ip= inet_ntoa (*(struct in_addr *)*hostinfo->h_addr_list);  //�������������ɹ��Ļ�������inet_ntoa()����ȡ��IP��ַ
			} 
		}
		IsGetWinsock_dll=TRUE;
		CreateSocket();		
	}
}//end of CreateConn()

void ConnectToSysCtrNode()
{                                                //��IsConnet��ֵ�жϲ��Խ����Ƿ������ܿؽڵ���̽�������
	if(IsGetWinsock_dll==FALSE)
	{
		CreateConn();
	}
	else
	{
		CreateSocket();
	}
}

int  SendAndRecvMsg(char * Message)                   //����ʵ�ֹ��ܣ�ʵ�����ܿؽڵ��ͨ��
{

	char Msg[20];
	int IsOk = FALSE;
	strcpy(Msg,Message);
	printf("��������");
     while(FALSE == IsOk)
	 {
		if(send(WinSocketClient,Message,strlen(Message),0)==-1)  //���ܿؽڵ���̷�������	
		{
				 IsConnet=FALSE;
				while(FALSE==IsConnet)
				{
					 CreateSocket();
					 Sleep(100);
					 printf("��");
				}
								
		}
		else
		 IsOk=TRUE;
	 }
	 printf("���\n");	
	 IsOk = FALSE;	 
	 printf("��������");
	 while(FALSE == IsOk)
	 {
		if( (sendchrcnt=recv(WinSocketClient,InputStr,20,0))==-1) //���������ܿؽڵ���̵����ݣ��ַ�������InputStr��Ž�������
		{
				IsConnet=FALSE;				
				while(IsConnet==FALSE)  
				{
					 CreateSocket();
					 Sleep(100);
					  printf("��");
				}
				if (TRUE == IsConnet)
				{
					send(WinSocketClient,Message,strlen(Message),0);

				}
		}
		else
			IsOk=TRUE;
			
	 }
	 printf("���\n");	
	   InputStr[sendchrcnt]='\0';
	return TRUE;
}//end of SendAndRecvMsg(char * Message)

int CheckMsgFromSysCtrNode()                 //����ʵ�ֹ��ܣ�����ܿؽڵ�Ļָ���Ϣ��������Ϣ���ж�
{

	if (InputStr[0]=='C'&&InputStr[1]=='S')//���и��ɽڵ�PlNode���������ر��ˣ������û����Ѿ����ͷ�(���ָܻ�)��
	{
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
		printf("û�и��ɽڵ���̿������������κ��û�����\n");
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 14);
		printf("û�и��ɽڵ㿪�������β��Ե��˽������û���ȫ����ʧ\n");	
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
		printf("���Խ��̽���5���ر�\n");
		Sleep(5000);				
			exit(0);
		return panduan1;
	}
	return panduan2;
}

#endif
