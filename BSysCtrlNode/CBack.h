int CBack;
char CBHR[2] = {"HR"};
char SystemNote[20] = {"SystemNote.txt"};
struct sockaddr_in ServerAddr,ClientAddr;	//struct sockaddr_in��internet�������׽��ֵĵ�ַ��ʽ
											//���������ɽڵ�ʱ��������
DWORD WINAPI ThreadCBack( LPVOID lpParam )//���߳�
{
	int accept_socket, AddrLen = 16, n =3;			//socket���.
	while(n--)
	{
		if ((accept_socket=accept(CBack,(struct sockaddr *)&ClientAddr,&AddrLen))==-1)
		{
			printf("\t\t���������ܿ�ʧ��!\n",n);
		}
		else
		{	printf("\t\t���������ܿسɹ�!\n\t\t��ʱ���ܿ���Ϊ����ʹ��!\n",n);break;	}
	}
	while(1)
	{
		if (send(accept_socket,CBHR,2,0)==-1)
		{
			printf("\t\t�������ܿؽڵ�ر�\n");
			break;
		}
		Sleep(10);
	}
	CreateThread( NULL, 0, ThreadCBack, NULL, 0, 0 );				//�������߳���ͼ���ڿ��������ܿؽ�������
}