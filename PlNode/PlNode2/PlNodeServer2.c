#include "PlNodeServer.h"
#include "Public.h"
//������
int main(int argc, char* argv[])
{
	int PlNode = 2;
	int i=0;
	int dport = 20002;
	printf("                                ��PlNode2��\n");

	//���ڴ�أ����������ID����ջ�ĳ�ʼ��
	allInit();
	//ͨ�ź���
	sockRun(dport,PlNode);
	
	return 0;
}


