#include "PlNodeServer.h"
#include "Public.h"

//������
int main(int argc, char* argv[])
{
	int PlNode = 5;
	int dport = 20005;
	printf("                                ��PlNode5��\n");

	//���ڴ�أ����������ID����ջ�ĳ�ʼ��
	allInit();
	//ͨ�ź���
	sockRun(dport,PlNode);
 
	return 0;
}


