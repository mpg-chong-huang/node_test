#include "PlNodeServer.h"
#include "Public.h"

//������
int main(int argc, char* argv[])
{
	int PlNode = 4;
	int dport = 20004;
	printf("                                ��PlNode4��\n");

	//���ڴ�أ����������ID����ջ�ĳ�ʼ��
	allInit();
	//ͨ�ź���
	sockRun(dport,PlNode);
 
	return 0;
}


