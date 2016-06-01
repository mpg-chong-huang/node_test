#include <winsock2.h>
#include <stdio.h>
#include <io.h>
#include <string.h>
#include <sys/types.h>
#include "RBTree4Ctr.h"
#define MAX 9999999
typedef struct
{
	int bool;								//�������Ƿ���
	int Num;								//��������������
}Server_Node;
											//���ɾ����㷨��ȫ�����ݱ���ͷ�ļ�
typedef struct								//��������������ṹ��
{
	Server_Node N[6];						//ABC�����������û�����
	int Total;								//�ܸ����û�����
	int MIN;								//��������ķ�������
} PIMES;
//............................................................
PIMES P={{{-1,MAX},{-1,MAX},{-1,MAX},{-1,MAX},{-1,MAX},{-1,MAX}},0,1};//��ʼ������������
int SocketIdagain[6];						//������socket	
int SocketHeart[6];							//������heart socket										
struct sockaddr_in ServerAddr,ClientAddr;	//struct sockaddr_in��internet�������׽��ֵĵ�ַ��ʽ
											//���������ɽڵ�ʱ��������
int accept_socket[6];						//��Ӧ�Ĳ��Խڵ㼰���ɽڵ�socket���.0���븱��������socket
int heart_socket[6];						//��Ӧ�Ĳ��Խڵ㼰���ɽڵ��heartsocket���.
DWORD WINAPI ThreadNewSocket( LPVOID lpParam );
char SeverBack[20] = {"PlNode.BU"};			//�����ļ�������
char HRZore[2] = {"HR"};					//���������ַ���
CRITICAL_SECTION g_cs;						//�߳�������
int change_ID;                              //ת���õ�ID
char severnum[6][20] = {"XXX","PlNode1","PlNode2","PlNode3","PlNode4","PlNode5"};

void TensionMin()//���㸺��ѹ����С�ķ��������
{
	int i = 0, j = 0, bool = 0;
	P.Total = 0;
	for( i =1 ; i < 6; i++ )
	{	P.Total += (P.N[i].bool != -1)?P.N[i].Num:0; }
	for( i = 1; i < 6; i++ )
	{
		bool = 0;
		for( j = 1; j < 6; j++)
		{
			if(P.N[i].Num > P.N[j].Num)
			{
				bool = 1;
			}
		}
		if( 0 == bool)
		{
			P.MIN = i;
			break;
		}
	}
}
void AgainConnect(u_short dport)			//�����ӻ���
{
	switch(dport)
	{
	case 10000:printf("\t\tTestProgress�Ͽ�����!\n\t\t����������TestProgress����:\n");break;
	default:printf("\t\tPlNode%d�Ͽ�����!\n\t\t����������PlNode%d����:\n",dport%10000,dport%10000);break;
	}
	accept_socket[dport%10000] = NewSocket(dport, SocketIdagain[dport%10000]);
}
int CheckSever(int n)						//���ڵ�n������񣬲����ھ�����Է���DS
{
	if(P.N[n].bool == -1)
	{
		if (send(accept_socket[0],"DS",2,0)==-1)//DS����ǰ���ؽڵ㲻����
		{
			AgainConnect(10000 );			//�����ӻ���
			return 0;
		}
		return 0;
	}	
	return -1;
}
void AssignServer ()						//���ؾ���
{
	P.N[P.MIN].Num++;
	P.Total++;
	TensionMin();
}
