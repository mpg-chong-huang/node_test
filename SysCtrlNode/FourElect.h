#include "FunctionHead.h"
										//���ֲ�������
int Addmodule(char Name[20], int bool)	//"+"�Ӳ���function/,1��ʾ��Ҫ���������ԣ�0��ʾ����Ҫ
{
	char Airing[20],Test[20];			//Air�㲥��
	int chcnt=0, i=0,id4File = -1;		//�����ַ���
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
loop:	if( 0 != bool)
		{
			if (CheckAllSever(1) == 0 )			//��һ�����ؽڵ㶼������
			exit(0);
		}
		TensionMin( ); //���ؾ���
		i = P.MIN;//���˴β���Ҫ����ĸ��ɽڵ����,����JudgeSafe
		if (send(accept_socket[P.MIN],Name,20,0)==-1)
			{	
				goto loop;
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
	int chcnt=0, i=0,n = 0;					//�����ַ��������ַ�����ת�Ƶĸ��ر��
	char uName[20];
	FILE *fpp = NULL;				//�����ļ���ȡָ��
	PRBTNode p = NULL;
	for(i=1; i<6; i++)//�㲥�������ؽڵ�ɾ����Ϣ
	{
		if (CheckAllSever(1) == 0 )			//��һ�����ؽڵ㶼������
			exit(0);
		{
			while(QueHead != QueTail)
			{//���ļ��������ļ������ļ�ָ���ַ
				EnterCriticalSection(&g_cs);
				outQueue(&n);//�õ������еĸ��ؽڵ�ı��
				LeaveCriticalSection(&g_cs);
				printf("\t\tת��PINode%d������......................................\n",n);
				tempTree(n);//��ʱ����ű�ת�Ƶ�����
				initRB(&tree[n]);//��ʼ����ת�Ƶ��Ǹ����ؽڵ��Ӧ����
				while(tree[0].root != tree[0].nil)					//����ͷ����ָ���ڱ��ڵ�
				{			
					p = deleteRB(&tree[0],tree[0].root);//ȡ��Ҷ�ӽڵ�
					strcpy(uName+1,p->Name);
					uName[0] = '+';
					change_ID = p->ID;
					Addmodule(uName,2);//��ȡ�������ݰ��������ת����ȥ
					Memory_recover(p);
				}
				initRB(&tree[0]);		   //��ʱ������ʹ������ʼ��
				printf("\t\tת��PINode%d�������....................................\n",n);
				printfNum( );	    	//��ӡ�ڵ����		
			}
		}
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
				delNode(Name+1,SeverBack,i);	//ɾ����Ӧ���Լ��ļ��е�����
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
	int chcnt=0, i=0,n = 0;			//�����ַ���
	char uName[20];
	FILE *fpp = NULL;				//�����ļ���ȡָ��
	PRBTNode p = NULL;
	for (i=1; i<6; i++) //�㲥�������ؽڵ��ѯ��Ϣ
	{
		if (CheckAllSever(1) == 0 )			//��һ�����ؽڵ㶼������
			exit(0);
		{
			while(QueHead != QueTail)
			{//���ļ��������ļ������ļ�ָ���ַ
				EnterCriticalSection(&g_cs);
				outQueue(&n);//�õ������еĸ��ؽڵ�ı��
				LeaveCriticalSection(&g_cs);
				printf("\t\tת��PINode%d������......................................\n",n);
				tempTree(n);//��ʱ����ű�ת�Ƶ�����
				initRB(&tree[n]);//��ʼ����ת�Ƶ��Ǹ����ؽڵ��Ӧ����
				while(tree[0].root != tree[0].nil)					//����ͷ����ָ���ڱ��ڵ�
				{			
					p = deleteRB(&tree[0],tree[0].root);//ȡ��Ҷ�ӽڵ�
					strcpy(uName+1,p->Name);
					uName[0] = '+';
					change_ID = p->ID;
					Addmodule(uName,2);//��ȡ�������ݰ��������ת����ȥ
					Memory_recover(p);
				}
				initRB(&tree[0]);		   //��ʱ������ʹ������ʼ��
				printf("\t\tת��PINode%d�������....................................\n",n);
				printfNum( );	    	//��ӡ�ڵ����		
			}
		}
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

