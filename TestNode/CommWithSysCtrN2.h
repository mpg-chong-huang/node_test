#include"Socket.h"
void ReleaseAllUser()
{
    int IsReleaseFlag = 0;
	unsigned int i = 0;
	if(FALSE == IsConnet)
	{
	   ConnectToSysCtrNode();
	}
		for(i=1;i<=5;i++)
		{
			Message[0] = 'R';                                       //����ʶ��'R'��ŵ��ַ�������Message����
			Message[1] = (char) (i+'0');                            //�����ɽڵ�PlNodeIDֵת��Ϊ�ַ���ŵ��ַ�������Message����
			Message[2] = '\0';
			if (SendAndRecvMsg(Message) == TRUE)                     //�ɺ���SendAndRecvMsg�ķ���ֵ���ж��Ƿ����ɺ��ܿؽڵ㽨������
			{
				if (panduan2 ==CheckMsgFromSysCtrNode())
				{
					if (InputStr[0]=='1')
					{
						IsReleaseFlag++;  
					}
					else if (InputStr[0]=='2')
					{
						//printf("���ɽڵ�PlNode%d���û��ͷ�ʧ��\n",i);
					}
					else if (InputStr[0]=='D'&&InputStr[1]=='S')
					{
						IsReleaseFlag++;
					}
				}
			}        				 			 						 	
		} 
		if (5==IsReleaseFlag)
		{
			printf("�����û���ȫ�����ͷ�\n");
		}
		else
		{
		  printf("�û�û��ȫ��ע���ɹ�\n");
		}
}

void  ShutdownSystem()
{
    int IsShutdownFlag = 0;
	unsigned int i ;
	printf("�������Զ��رո��ɽڵ�����ܿؽڵ����Ͳ��Խ��̳���\n");
	Sleep(500);	
	if(FALSE == IsConnet)
	{
	   ConnectToSysCtrNode();
	}
	for(i=1;i<=5;i++)
	{
		Message[0] = 'S';                                       //����ʶ��'R'��ŵ��ַ�������Message����
		Message[1] = (char) (i+'0');                           //�����ɽڵ�PlNodeIDֵת��Ϊ�ַ���ŵ��ַ�������Message����
		Message[2] = '\0';
		if (SendAndRecvMsg(Message) == TRUE)                  //�ɺ���SendAndRecvMsg�ķ���ֵ���ж��Ƿ����ɺ��ܿؽڵ㽨������
		{
			if (panduan2 ==CheckMsgFromSysCtrNode())
			{
				if (InputStr[0]=='1')
				{
					IsShutdownFlag++;  
				}
				else if (InputStr[0]=='2')
				{
					//printf("���ɽڵ�PlNode%d���û��ͷ�ʧ��\n",i);
				}
				else 
				{
					IsShutdownFlag++;
				}
			}
		} 
		Sleep(500);
	} 
	if (5==IsShutdownFlag)
	{
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);                  //����������ɫΪ����
		printf("���Խ��̽���3���ر�\n");
		Sleep(3000);							
		exit(0);
	}
	else
	{
	   printf("�ر�ϵͳʧ��\n");
	}
}



