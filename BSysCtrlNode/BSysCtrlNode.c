// SysCtrlNode.cpp : Defines the entry point for the console application.
//
#include "SysCtrlcode.h"

int main(int argc, char* argv[])
{	
	FILE *fp;
	char temp[31];
	int BSocket = 0;
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);//����printf��ɫ
	printf("\t\t     _________\n"); 
	printf("\t\t    / ======= \\ \n");
	printf("\t\t   / __________\\ \n");
	printf("\t\t  | ___________ | \n");
	printf("\t\t  | |  Hello  | | \n");
	printf("\t\t  | |  -   -  | | <<���ɷֵ��Ŀɿ��Ա����ܿ�>>\n");
	printf("\t\t  | |_________| |_______________ \n");
	printf("\t\t  \\_____________/                )\n"); 
	printf("\t\t  / =========== \\              /        \n");
	printf("\t\t / ::::::::::::: \\         =D-'\n");
	printf("\t\t(_________________)\n");
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 12);//����printf��ɫ
	//��ʼ���ڴ��еĺ����											
	//��ʼ��IDջ
	//�ڴ�س�ʼ��
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 11);//����printf��ɫ
	BSocket = BSocketInit(20010);
	Connect2CtrlNode(BSocket);
	BClientHeart( BSocket );
	InitializeCriticalSection(&g_cs);							 //�����߳���
	//ʹ���ܿ���Ϊ�����ã�Ӧ����Ҫ�ö��̵߳�
	Sleep(100);
	GetSocketId(20000, 3);
	CreateThread( NULL, 0, ThreadCBack, NULL, 0, 0 );				//�������߳���ͼ���ڿ��������ܿؽ�������
	Sleep(100);
	InItiate(1, 6);	
	allinit();	//��ʼ�����ؽڵ��Լ�����
	Sleep(100);
	printf("\t\t�����н���Test�Լ�PLNode(1~5)����:\n");
	GetSocketId(10000, 1);
	if (!openFile(SeverBack,&fp))						  //���ļ����ݴ���
	{//ʧ��
		printf("\t\t��ʧ��\n");
	} 
	else
	{//�ɹ�
		//���ļ��������ļ�ָ�룬�ַ�ָ���ַ
		printf("\t\t��ʼ��������,�����ĵȴ�................\n");
		while(!readFromFile(fp,temp))					//ȫ����ȡά��һ���ڴ�����
		{		
			
			temp[0] = '+';
			if( temp[1]!= '*' && temp[1] != '\0')
			{
				Addmodule(temp, 0);						//�����ļ���ȡ��ÿһ������ӽڵ㷢�͸������
			}
		}
		//�ر��ļ�
		closeFile(fp);
		printf("\t\t��ʼ�����,ϵͳ������..................\n");
	}
	Sleep(100);
	accept_socket[0] = NewSocket(10000, SocketIdagain[0]);//���Խڵ�������߳�
	printfNum( );										//��ӡ���ؽڵ�
	SysCtrlNodeSocket( );								//����ģ��
	return 0;
}
