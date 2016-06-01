#include "Socket.h"
int UserNameCheck (char * Input)                                //����ʵ�ֹ��ܣ��ж��û����Ƿ�Ϸ�
{
	unsigned int i=0;
	if( (strlen(Input)==0)|| ( strlen (Input)>16 || strlen(Input)<4 ) ||Input[0]=='_' )
	{                                                             
		return	FALSE;                                          //����û�������4-16���ַ�֮�ڻ��û�����һ���ַ����»��ߣ�����FALSE
	}
	while( i< strlen(Input) )
	{
		if (  Input[i]=='_' || (Input[i]>='0'&&Input[i]<='9') || (Input[i]>='a'&&Input[i]<='z') || (Input[i]>='A'&&Input[i]<='Z') )
		{
			i++;
		}
		else
		{
			return 	FALSE;	                                     //����û������ַ�ȡֵ������Ӣ����ĸ�����ֻ��»��ߣ�����FALSE
		}
	}
	return TRUE;                                                 //����û������ַ�ȡֵ��Ӣ����ĸ�����ֻ��»��ߣ�����TRUE	
}//end of UserNameCheck (char * Input) 

void ConnectReq(char * UserName)                                 //����ʵ�ֹ��ܣ���SysCtrlNode���̷���Ϣ֪ͨ�û�UserName����
{	       
    unsigned int i;
	if (UserNameCheck(UserName)==FALSE)                           
	{                                                            //���UserNameCheck�����ķ���ֵΪFALSE����ʾ�����û�ʧ��
		printf("�û���'%s'���Ϸ�,�����û�ʧ�ܡ�\n",UserName);                                      
	}
	else                                                         //���UserNameCheck�����ķ���ֵΪTRUE��ִ���������
	{
			if(FALSE == IsConnet)
			{
			   ConnectToSysCtrNode();
			}    				 
				Message[0] = '+';                                 //����ʶ��'+'��ŵ��ַ�������Message����
				for (i=0; i<=strlen(UserName); i++)               //Ҫ�û�����ŵ��ַ�������Message��
					Message[i+1]=UserName[i];
					if (SendAndRecvMsg(Message) == TRUE)           //�ɺ���SendAndRecvMsg�ķ���ֵ���ж��Ƿ����ɺ��ܿؽڵ㽨������
					{							
							if(panduan2 == CheckMsgFromSysCtrNode())
							{
                                    if (InputStr[0]=='2')                                        
									{
										printf("�û�%s�ѽ������\n",UserName);    //InputStr[0]ֵΪ2,��ʾ�½��û��������û�����
									}
									else if (InputStr[0]=='1')                    
									{
										printf("�û�%s�����ɹ�,�ѽ������\n",UserName);    //InputStr[0]ֵΪ1,��ʾ�û������ɹ�
									}
							}
					}   				 			 						 	
			
	}     //end of if (UserNameCheck(UserName)==FALSE)
}        
//end of ConnectReq(char * UserName) 

void ReleaseReq(char * UserName)                                //����ʵ�ֹ��ܣ���SysCtrlNode���̷���Ϣ֪ͨ�û�A�ͷ�
{
	unsigned int i;  
	if (UserNameCheck(UserName)==FALSE)
	{
		printf("�û���'%s'���Ϸ���������Ҫ�ͷŵ��û�%s\n",UserName,UserName);        //���UserNameCheck�����ķ���ֵΪFALSE����ʾҪ�ͷŵ��û����Ϸ�
	}
	else                                                        //���UserNameCheck�����ķ���ֵΪTRUE��ִ���������
	{
			if(FALSE == IsConnet)
			{
			   ConnectToSysCtrNode();
			}		
			Message[0] = '~';                                   //����ʶ��'~'��ŵ��ַ�������Message����			
			for (i=0; i<=strlen(UserName); i++)                 //Ҫ�û�����ŵ��ַ�������Message��
						Message[i+1]=UserName[i];
			if ( TRUE == SendAndRecvMsg(Message))
			{
					if(panduan2 == CheckMsgFromSysCtrNode())				
					{
                            if (InputStr[0]=='3')                                    
							{
								printf("Ҫ�ͷŵ��û�%s������\n",UserName);  //���InputStr[0]ֵΪ3,��ʾҪ�ͷŵ��û�������
							}
							else if (InputStr[0]=='1')                              
							{
								printf("�ɹ��ͷ��û�%s\n",UserName);        //���InputStr[0]ֵΪ1,��ʾ�ɹ��ͷ��û�
							}
					}
			}    //end of if(IsConnet2 == TRUE)
	}    //end of if (UserNameCheck(UserName)==FALSE)		

}  

int GetPnodeByUser(char * UserName)                                //����ʵ�ֹ��ܣ������û�UserName��Pnode��ʶ,��PnodeId��
{
	unsigned int i=0;       
	if (UserNameCheck(UserName)==FALSE)
	{
		printf("�û���'%s'���Ϸ���������Ҫ��ѯ���û�%s\n",UserName,UserName); //���UserNameCheck�����ķ���ֵΪFALSE����ʾҪ��ѯ���û����Ϸ�
		return -1;                                          //������Чֵ-1
	}
    //UserNameCheck�����ķ���ֵΪTRUE����ִ���������
	if(FALSE == IsConnet)
	{
	   ConnectToSysCtrNode();
	}
	Message[0] = '#';                                               //����ʶ��'#'��ŵ��ַ�������Message����
	for (i=0; i<=strlen(UserName); i++)                             //Ҫ�û�����ŵ��ַ�������Message��
	{	  
		Message[i+1]=UserName[i];
	}
	if ( TRUE == SendAndRecvMsg(Message))
	{
			if(panduan1 == CheckMsgFromSysCtrNode())
			{
				return -1;
			}
            //����CheckMsgFromSysCtrNode()�ķ���ֵΪpanduan2����ִ���������
  			if (InputStr[0]=='0')               
			{				                                        //���InputStr[0]ֵΪ0,��ʾ�û�����5��Pnode�ڵ���
				printf("Ҫ��ѯ���û�%s������\n",UserName);
				return -1;                                          //������Чֵ-1
			}
			else                             
			{                                                       //����InputStr[0]ֵ��ʾ�û�����Pnode�ڵ�                                   				
				return atoi(InputStr);                                  //�����û�����Pnode�ڵ�
			}
	}				
    return -1;
}

unsigned int GetUserNumByPnode(int PlnodeID)                        //���ظ��ɽڵ�PlNode���̵�ǰ������û���
{         			
	if (PlnodeID>5||PlnodeID<=0)                                    //�жϸ��ɽڵ�PlNodeIDֵ�Ƿ���1-5��
	{
		printf("���븺�ɽڵ��ID'%d'���Ϸ��������ڸ��ɽڵ�Plnode%d\n",PlnodeID,PlnodeID);
		return	0;                                                  //������ɽڵ�PlNodeIDֵ���Ϸ�������0
	}
     //���ɽڵ�PlNodeIDֵ��Ч��ִ���������
	if(FALSE == IsConnet)
	{
	   ConnectToSysCtrNode();
	}
	Message[0] = '@';                                               //����ʶ��'#'��ŵ��ַ�������Message����
	Message[1] = (char) (PlnodeID+'0');                             //�����ɽڵ�PlNodeIDֵת��Ϊ�ַ���ŵ��ַ�������Message����
	Message[2] = '\0';
	if (TRUE == SendAndRecvMsg(Message))
	{
		if(panduan2 ==CheckMsgFromSysCtrNode())
		{
			if (InputStr[0]=='0')               
			{				                                        //���InputStr[0]ֵΪ0,��ʾ�û�����5��Pnode�ڵ���
				printf("���ɽڵ�Plnode%d��ǰ������û���Ϊ0\n",PlnodeID);
				return 0;                                           //������Чֵ0
			}
			if (InputStr[0]=='D'&&InputStr[1]=='S')                 //���InputStr[0]=='D'&&InputStr[1]=='S',��ʾ���ɽڵ�û�д�
			{
				printf("���ɽڵ�Plnode%dû�д򿪣�Plnode%dû���û�����\n",PlnodeID,PlnodeID);
			}
			else                             
			{                                                       //����atoi(InputStr)ֵ��ʾ�û�����Pnode�ڵ�                                   				
				return atoi(InputStr);                              //�����û�����
			}
		}			
	}       
    return 0;
}
