#include"Socket.h"
/*
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
		Message[1] = (char) (i+'0');                      //�����ɽڵ�PlNodeIDֵת��Ϊ�ַ���ŵ��ַ�������Message����
		Message[2] = '\0';
		    //printf("�����ͷ��û��У����Ժ�\n");
			if (SendAndRecvMsg(Message) == TRUE)                  //�ɺ���SendAndRecvMsg�ķ���ֵ���ж��Ƿ����ɺ��ܿؽڵ㽨������
			{
			    if (panduan2 ==CheckMsgFromSysCtrNode())
				{
					if (InputStr[0]=='1')
					{
						IsReleaseFlag++;//printf("�ɹ��ͷŸ��ɽڵ�PlNode%d�������û�\n",i);  
					}
					else if (InputStr[0]=='2')
					{
						//printf("���ɽڵ�PlNode%d���û��ͷ�ʧ��\n",i);
					}
                    else if (InputStr[0]=='D'&&InputStr[1]=='S')
					{
						IsReleaseFlag++;//printf("���ɽڵ�PlNode%dû�д�\n",i);
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


int  ShutdownSystem()
{
    int IsShutdownFlag = 0;
	//int IsShutdownAllPn = FALSE;
	unsigned int i ;
	if(FALSE == IsConnet)
	{
	   ConnectToSysCtrNode();
	}
	for(i=1;i<=5;i++)
	{

		Message[0] = 'S';                                       //����ʶ��'R'��ŵ��ַ�������Message����
		Message[1] = (char) (i+'0');                      //�����ɽڵ�PlNodeIDֵת��Ϊ�ַ���ŵ��ַ�������Message����
		Message[2] = '\0';
		
		
		//printf("�����ͷ��û��У����Ժ�\n");
		if (SendAndRecvMsg(Message) == TRUE)                  //�ɺ���SendAndRecvMsg�ķ���ֵ���ж��Ƿ����ɺ��ܿؽڵ㽨������
		{
            //if (panduan1 ==CheckMsgFromSysCtrNode())
		//	{
			//	printf("���β��Ե��˽��������ر�ϵͳ��ע�������û�\n");
			//	Sleep(500);
				
			//	 exit(0);
			//}
			if (panduan2 ==CheckMsgFromSysCtrNode())
			{
				if (InputStr[0]=='1')
				{
					IsShutdownFlag++;//printf("�ɹ��ͷŸ��ɽڵ�PlNode%d�������û�\n",i);  
				}
				else if (InputStr[0]=='2')
				{
					//printf("���ɽڵ�PlNode%d���û��ͷ�ʧ��\n",i);
				}
				else //if (InputStr[0]=='D'&&InputStr[1]=='S')
				{
					IsShutdownFlag++;//printf("���ɽڵ�PlNode%dû�д�\n",i);
				}

			}

		} 
		else 
		{
		 return TRUE;
		}
		Sleep(500);
	} 
	if (5==IsShutdownFlag)
	{
		//printf("�����û���ȫ�����ͷ�\n");
		return TRUE;
	}
	else
	{
	   //printf("�û�û��ȫ��ע���ɹ�\n");
	   return FALSE;
	}
}

  */