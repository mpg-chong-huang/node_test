#include "FunctionHead.h"
										//���ֲ�������
int Addmodule(char Name[20], int bool)	//"+"�Ӳ���function/,1��ʾ��Ҫ���������ԣ�0��ʾ����Ҫ
{
	char Airing[20],Test[20];			//Air�㲥��
	int chcnt=0, i=0,id4File = -1;;					//�����ַ���
		strcpy(Airing, Name);
		Airing[0] = '_';
		if(2 != bool)
		{
				for (i=1; i<6; i++) 
				{
					if( 0 != bool)
					{
						if (CheckAllSever(1) == 0 )		//��һ�����ؽڵ㶼������
						exit(0);
					}
					if(0 == P.N[i].bool)			//�˸��ؽڵ����
					{
							if (send(accept_socket[i],Airing,20,0)==-1)
								{
									continue;
								}
							if ((chcnt=recv(accept_socket[i],Test,20,0))==-1)	//ʹ��accept�������ص�socket��!
								{
									continue;
								}
						Test[chcnt]='\0';
						if ('2' == Test[0])//����˵��������
						{
							if(0 == bool)//����-1�Ƕ�̬����ID������Ҫ������ID
							write2RBTree(Name+1,i,sum_id-1);//�����ڴ�
							break;//һ��������������ѭ��
						}
					}
			}
		}
		else{i = 6;}
	if (6 == i)	//˵���㲥���������ؽڵ�����������ҵ���ȷ�ɲ���ĸ��ɽڵ�
	{
loop1:	if( 0 != bool)
		{
			if (CheckAllSever(1) == 0 )			//��һ�����ؽڵ㶼������
			exit(0);
		}
		TensionMin( ); //���ؾ���
		i = P.MIN;//���˴β���Ҫ����ĸ��ɽڵ����,����JudgeSafe
		if (send(accept_socket[P.MIN],Name,20,0)==-1)
			{	
				goto loop1;
			}
		else 
		{
			if(0 == bool)//����-1�Ƕ�̬����ID������Ҫ������ID
				write2RBTree(Name+1,P.MIN,sum_id-1);//�����ڴ�
			if(2 == bool)//ת����
			{
				changeTree(Name+1,P.MIN,change_ID);
			}
			if(1 == bool)
			{
				id4File = write2RBTree(Name+1,P.MIN,-1);//�����ڴ�
				backup2File(Name+1,SeverBack,id4File);//��ӽ��ļ�	
			}
		}
		if ((chcnt=recv(accept_socket[P.MIN],Name,20,0))==-1)	//ʹ��accept�������ص�socket��!
			{
				strcpy(Name, "1");
			}
		AssignServer ();	//���ؾ���
		Name[chcnt]='\0';	//�����ַ���������.
		if(1 == bool)
		{
			if (send(accept_socket[0],Name,20,0)==-1)//���½��û��ɹ�����Ϣ���������Խڵ�
			{
				AgainConnect(10000 );//�����ӻ���
				return -1;
			}
			JudgeSafe( i, Name );
		}
	}
	else  //�������޷�����
	{
		if(1 == bool)
		{
			if (send(accept_socket[0],Test,20,0)==-1)
			{
				AgainConnect(10000 );//�����ӻ���
				return -1;
			}
		printf("  System Safe!\n");//��ʱ��Ӳ��ɹ���ϵͳһ����safe��
		}
	}
	TensionMin( ); //���ؾ���
	return -1;
}
int Omitemodule(char Name[20])	//"~"ɾ������function
{
	char Test[20];			//Air�㲥��
	int chcnt=0, i=0;					//�����ַ���
	for(i=1; i<6; i++)//�㲥�������ؽڵ�ɾ����Ϣ
	{
		if (CheckAllSever(1) == 0 )			//��һ�����ؽڵ㶼������
			exit(0);
		if(0 == P.N[i].bool)
		{
			if (send(accept_socket[i],Name,20,0)==-1)
			{
				continue;
			}
			if ((chcnt=recv(accept_socket[i],Test,20,0))==-1)	//ʹ��accept�������ص�socket��!
			{
				continue;
			}
			Test[chcnt]='\0';					//�����ַ���������.
			if('1' == Test[0])					//ɾ���ɹ�ʱ
			{
				P.N[i].Num--;P.Total--;
				delNode(Name+1,SeverBack,i);
				TensionMin( ); //���ؾ���
				JudgeSafe( i , Test );
				break;
			}
		}
	}
	if(6 == i)
	{
		printf("  Load Balancing  System Safe!\n");//��ʱɾ�����ɹ���ϵͳһ����safe��
	}
	if (send(accept_socket[0],Test,20,0)==-1)//����ɾ���ɹ���񶼻���һ����Ϣ���������Խڵ�
	{
		AgainConnect(10000 );				 //�����ӻ���
		return -1;							 //���Խ�������ʱ����ֵ
	}
	TensionMin( ); //���ؾ���
	return 0;								 //����ʱ����ֵ
}
int refermodule(char Name[20])				//"#"��ѯ����function
{
	char Test[20] = "0";			//Air�㲥��
	int chcnt=0, i=0;			//�����ַ���
	for (i=1; i<6; i++) //�㲥�������ؽڵ��ѯ��Ϣ
	{
		if (CheckAllSever(1) == 0 )			//��һ�����ؽڵ㶼������
			exit(0);
		if(0 == P.N[i].bool)
		{
			if (send(accept_socket[i],Name,20,0)==-1)
				{
					continue;
				}
			if ((chcnt=recv(accept_socket[i],Test,20,0))==-1)	//ʹ��accept�������ص�socket��!
				{
					continue;
				}
			Test[chcnt]='\0';
			if ('0' != Test[0])//��ʱ�ҵ����û�����һ�����ɽڵ���
				{
					break;
				}
		}
	}
	if (send(accept_socket[0],Test,20,0)==-1)//���۲�ѯ�Ƿ�ɹ��������Խڵ�һ����Ϣ
		{
			AgainConnect(10000 );		//�����ӻ���
			return -1;					//���Խ�������ʱ����ֵ
		}
	return 0;							//����ʱ����ֵ
}
int Amodule(char Name[20])				//"@"��ѯ����function
{
	int chcnt=0,ServerN=0;				//�����ַ���
	ServerN = (int)(Name[1])-48;		//��ȡ����ѯ�ĸ��ɽڵ�ı��
	printf("\t\t��ѯ(%d)���ɽڵ��û�������:\n",ServerN);
	if (CheckSever(ServerN) == 0)		//����ǰ@�ĸ��ؽڵ㲻����
		return 0;
	if (send(accept_socket[ServerN],Name,20,0)==-1)
		{
			if (send(accept_socket[0],"DS",2,0)==-1)//��@��ѯ���Ľ�����������Խڵ�
			{
				AgainConnect(10000 );				//�����ӻ���
				return -1;
			}
		}
	if ((chcnt=recv(accept_socket[ServerN],Name,20,0))==-1)	//ʹ��accept�������ص�socket��!
		{
			if (send(accept_socket[0],"DS",2,0)==-1)//��@��ѯ���Ľ�����������Խڵ�
			{
				AgainConnect(10000 );				//�����ӻ���
				return -1;
			}
		}
	Name[chcnt]='\0';
	if (send(accept_socket[0],Name,20,0)==-1)//��@��ѯ���Ľ�����������Խڵ�
		{
			AgainConnect(10000 );				//�����ӻ���
			return -1;
		}
	return 0;
}

