// SysCtrlNode.cpp : Defines the entry point for the console application.
//
#include "SysCtrlcode.h"

int main(int argc, char* argv[])
{	
	FILE *fp;
	char temp[31];
	BSocket = BSocketInit(20000);								
	Connect2CtrlNode( );										 //��Ϊ���ݷ�������,���������
	InitializeCriticalSection(&g_cs);							 //�����߳���
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);//����printf��ɫ
	printf("\n\t\t     _________\n"); 
	printf("\t\t    / ======= \\ \n");
	printf("\t\t   / __________\\ \n");
	printf("\t\t  | ___________ | \n");
	printf("\t\t  | |  Hello  | | \n");
	printf("\t\t  | |  -   -  | | <<���ɷֵ��Ŀɿ���ϵͳ����>>\n");
	printf("\t\t  | |_________| |_______________ \n");
	printf("\t\t  \\_____________/                )\n"); 
	printf("\t\t  / =========== \\              /        \n");
	printf("\t\t / ::::::::::::: \\         =D-'\n");
	printf("\t\t(_________________)\n");
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 12);//����printf��ɫ
	while(BBack == 1)
	{
		Sleep(100);												//��������Ϊ����ʱ��ס
	}
	//��ʼ���ڴ��еĺ����											
	//��ʼ��IDջ
	//�ڴ�س�ʼ��
	Sleep(150);
	InItiate(0, 6);												//��ʼ�����ؽڵ��Լ�����
	allinit();
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 11);//����printf��ɫ
	printf("\t\t�����н���PLNode(1~5)�����Լ�Test����BSys�����ؽڵ�:\n");
	printf("\t\t���Զ���:\n");
	Sleep(100);
	GetSocketId(10000, 1);
	if (!openFile(SeverBack,&fp))						  //���ļ����ݴ���
	{//ʧ��
		printf("\t\t��ʧ��\n");
	} 
	else
	{//�ɹ�
		//���ļ��������ļ�ָ�룬�ַ�ָ���ַ
		printf("\t\t��ʼ��������,�����ĵȴ�...................................\n");
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
		printf("\t\t��ʼ�����,ϵͳ������......................................\n");
	}
	Sleep(100);
	accept_socket[0] = NewSocket(10000, SocketIdagain[0]);//���Խڵ�������߳�\
	printfNum( );										//��ӡ���ؽڵ�
	SysCtrlNodeSocket( );								//����ģ��
	return 0;
}
