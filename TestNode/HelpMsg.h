#include "Socket.h"
#ifndef _HELPMSG_H
#define _HELPMSG_H
void printPrefaceMessage()
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);                  //����������ɫΪ����
	printf("\t\t\t\t�����Խ��̡�\nʹ�����밴Ҫ������Ϸ�����\n");	
	printf("��Ҫ������������\n");
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 10); 
	printf("1�������û�A, �������+A \n");
	printf("2���ͷ��û�A, �������~A \n");
	printf("3����ѯ�����û�A�ĸ��ɽڵ�PlNode��ID, �������#A \n");
	printf("4����ѯ���ɽڵ�PlNodei���û�����, �������@i \n");	
    printf("5�������½�����ѯ���ͷ�n���û����������CGR ���س�����������ֵn  \n");
	printf("6��ע�������û����������LOGOUT\n");
	printf("7���رղ��Խ��̣��������EXIT\n");
	printf("8���˳�����ϵͳ���������SHUTDOWN\n");
	printf("9���鿴������ϸʹ��˵������������:HELP\n");
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);                  //����������ɫΪ����
	printf("A��ʾ�û���,��ȡֵ������4-16���ַ�֮��,�ַ�ֻ����Ӣ���ַ������ֺ��»���,��1���ַ�����Ϊ�»���.\n");		
	printf("i��ʾ���ɽڵ�PlNode��ʶ,��ȡֵֻ����1��2��3��4��5\n");
	printf("n��ʾ1����ֵ����ȡֵΪ��0<n<=1000\n");
	printf("����֧��Ӣ���ַ���Сд���롣������CGR������cgr��˼һ��\n");
	printf("����ÿִ����һ������󣬽��ȴ�ʹ��������������\n");
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 14); 
	printf("�տ�ʼ���в���ʱ���뿪��SysCtrPlnode.exe���򣬷������û����\n");	
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 11); 
	printf("��������ʱ�����ڹرղ��Խ���ǰʹ��SHUTDOWN��LOGOUT���\n");
    printf("�����ֶ�ɾ��PlNode.BU�ļ����Ա�֤��һ�β��Ի����Ǵ�����\n");
    
}

void printHelpMessage()
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 10);
	printf("===================== ��������˵����Ϣ ======================\n");
	printf("����           ����˵����ʹ��˵��\n");
	printf("HELP           �ṩ���������˵����Ϣ\n\n");
	printf("+              �����û�,���½��û�A�����룺+A\n");
	printf("               A��ʾ�û���,��ȡֵ������4-16���ַ�֮��,�ַ�ֻ����Ӣ����ĸ��\n");
	printf("               ���ֺ��»���,��1���ַ�����Ϊ�»���\n\n");
	printf("~              �ͷ��û�A,�����磺~A\n");
	printf("               A��ʾ�û���,��ȡֵ������4-16���ַ�֮��,\n");
	printf("               �ַ�ֻ����Ӣ����ĸ�����ֺ��»���,��1���ַ�����Ϊ�»���\n\n");
	printf("#              ��ѯ�û�A���ڸ��ɽڵ�PlNode��ID,�����磺#A\n");
	printf("               A��ʾ�û���,��ȡֵ������4-16���ַ�֮��,\n");
	printf("               �ַ�ֻ����Ӣ���ַ������ֺ��»���,��1���ַ�����Ϊ�»���\n\n");
	printf("@              ��ѯ���ɽڵ�PlNodei���û�����,�����磺@i\n");
	printf("               i��ʾ���ɽڵ�PlNode��ʶ,��ȡֵֻ����1��2��3��4��5\n\n");																	
	printf("CGR            �����½�����ѯ���ͷ�n���û�,���½�����ѯ���ͷ�10�û�,\n");
	printf("               ���룺CGR ,�س���������:10\n");
	printf("               n��ȡֵΪ��0<n<=1000���û�����1000-100000���������\n");
	printf("LOGOUT         ע�������û�����,��ע�����û����񲻻�ָ�\n\n");
	printf("EXIT           �رղ��Խ���\n\n");
    printf("SHUTDOWN       �رշ���ϵͳ�Ͳ��Խ���\n\n");
	printf("======================= ˵����Ϣ���� ========================\n");		
}

#endif