#include "PlNodeServer.h"
#include "Public.h"

//������
int main(int argc, char* argv[])
{
	int PlNode = 3;
	int i=0;
	int dport = 20003;
	printf("                                ��PlNode3��\n");
	
	//���ڴ�أ����������ID����ջ�ĳ�ʼ��
	allInit();
	//ͨ�ź���
	sockRun(dport,PlNode);
	
	return 0;
}


