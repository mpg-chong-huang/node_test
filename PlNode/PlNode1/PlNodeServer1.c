#include "PlNodeServer.h"
#include "Public.h"

//������
int main(int argc, char* argv[])
{
	int PlNode = 1;
	int dport = 20001;
	printf("                                ��PlNode1��\n");

	//���ڴ�أ����������ID����ջ�ĳ�ʼ��
	allInit();
	//ͨ�ź���
	sockRun(dport,PlNode);
 
	return 0;
}


