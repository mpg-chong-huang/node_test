#include "FourElect.h"
//���ڵ��ʼ������
int SysCtrlNodeSocket( )			//�ܿ�ģ�麯��socketͨ�ź���
{
	char Name[20],Air[20],uName[20];//Air�㲥��
	int chcnt=0, n = 0, i =0;		//�����ַ�����ת�Ƶĸ��ر��
	FILE *fpp = NULL;				//�����ļ���ȡָ��
	PRBTNode p = NULL;
	while (1)	
	{ 
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 12);//����printf��ɫ
				if ((chcnt=recv(accept_socket[0],Name,20,0))==-1)//ʹ��accept�������ص�socket��!
				{
					AgainConnect(10000 );//�����ӻ��� 
					continue;
				}
				Name[chcnt]='\0';		//�����ַ���������.
				if(Name[0] !='R' && Name[0] != 'S')
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
						switch(Name[0])			//�����߼��жϣ�
						{
						case '+':				//����û�
							{
								printf("\t\t����û�����:");
								if (CheckAllSever(1) == 0 )			//��һ�����ؽڵ㶼������
									exit(0);
								Addmodule(Name, 1);//���û�
								break;
							}
						case '~':				//ɾ���û�
							{
								printf("\t\tɾ���û�����:");
								if (CheckAllSever(1) == 0 )			//��һ�����ؽڵ㶼������
									exit(0);
								Omitemodule(Name);//ɾ���û�	
								break;
							}
						case '#':				 //��ѯ���û�����һ�����ɽڵ���
							{	//��Ϊ��ѯ����һ���ǰ�ȫ��
								printf("\t\t��ѯ�û����ڸ��ɽڵ����:\n\t\tLoad Balancing  System Safe!\n");
								if (CheckAllSever(1) == 0 )			//��һ�����ؽڵ㶼������
									exit(0);
								refermodule(Name);
								break;
							}
						case '@':				//�鸺�ɽڵ��ʱ�Ľڵ���
							{
								printf("\t\tLoad Balancing  System Safe!\n");     //��Ϊ��ѯ��������һ���ǰ�ȫ��
								if (CheckAllSever(1) == 0 )			//��һ�����ؽڵ㶼������
									exit(0);
								Amodule(Name);  //�鸺�ɽڵ��ʱ�Ľڵ���
								break;
							}
						case 'R':				//�鸺�ɽڵ��ʱ�Ľڵ���
							{
								printf("\t\tע������\n\t\tLoad Balancing  System Safe!\n");     //��Ϊע����������һ���ǰ�ȫ��
								if (CheckAllSever(1) == 0 )			//��һ�����ؽڵ㶼������
									exit(0);
								LogOff(Name);
								break;
							}
						case 'S':				//�鸺�ɽڵ��ʱ�Ľڵ���
							{
								printf("\t\t�˳�����\n\t\tLoad Balancing  System Safe!\n");     //��Ϊ�˳���������һ���ǰ�ȫ��
								ShutDown(Name);
								if('5' == Name[1])
								{
									strcpy(CBHR, "SD");
									write2Log("���ؾ���ϵͳ",SystemNote,0);//0�ǶϿ�
									printf("\t\t1��� Shutdown������\n");
									Sleep(1000);
									exit(0);
								}
								break;
							}
						default: break;
						}	
					printfNum( );									//��ӡ�ڵ����
			}
		WSACleanup( );//ж��Winsock�⣬���ͷ�������Դ
	return 0;
}
int CheckAllSever( )						   //��⸺�ؽڵ��Ƿ�һ���������ڣ������ھ�����Է���CS
{
	int i = 0;
	for ( i = 1; i< 6; i++)
	{
		if(P.N[i].bool == 0)
			break;
	}
	if(i == 6)									//����һ��������
	{
		allClean(SeverBack);					//���ȫ�����ݰ��������ļ�
		printf("\n\t\tû�и��ؽڵ����ӵ�ϵͳ,ϵͳ����!!\n");
		if (send(accept_socket[0],"CS",20,0)==-1)//CS�����ؽڵ�һ����������
		{
			AgainConnect(10000 );				//�����ӻ���
			return 0;							//0��ʾ����һ����������
		}
		printf("\t\tû�и��ؽڵ����,ϵͳ�˳�!!\n");
		strcpy(CBHR, "SD");
		write2Log("���ؾ���ϵͳ",SystemNote,0);//0�ǶϿ�
		Sleep(2000);
		return 0;
	}	
	return -1;									//-1��ʾ���ٻ���һ������
}
int LogOff(char Name[20])				//Rע������function
{
	int chcnt=0,ServerN=0;			     //�����ַ���
	FILE *fp = NULL;
	ServerN = (int)(Name[1])-48;		//��ȡ��ע���ĸ��ɽڵ�ı��
	if (CheckSever(ServerN) == 0)		//����ǰL�ĸ��ؽڵ㲻����
		return 0;
	if (send(accept_socket[ServerN],Name,20,0)==-1)
	{
		if (send(accept_socket[0],"DS",20,0)==-1)//��L���Ľ�����������Խڵ�
		{
			AgainConnect(10000 );				//�����ӻ���
			return -1;
		}
	}
	if ((chcnt=recv(accept_socket[ServerN],Name,20,0))==-1)	//ʹ��accept�������ص�socket��!
	{
		if (send(accept_socket[0],"DS",20,0)==-1)//��R��ѯ���Ľ�����������Խڵ�
		{
			AgainConnect(10000 );				//�����ӻ���
			return -1;
		}
	}
	P.N[ServerN].Num = 0;
	TensionMin( ); //���ؾ���
	Name[chcnt]='\0';
	if (send(accept_socket[0],Name,20,0)==-1)//��R��ѯ���Ľ�����������Խڵ�
	{
		AgainConnect(10000 );				//�����ӻ���
		return -1;
	}
	allClean(SeverBack);					//���ȫ�����ݰ��������ļ�				
	return 0;
}

int ShutDown(char Name[20])				//S�˳�ϵͳ����function
{
	int chcnt=0,ServerN=0;			     //�����ַ���
	FILE *fp = NULL;
	ServerN = (int)(Name[1])-48;		//��ȡ��ע���ĸ��ɽڵ�ı��
	if (CheckSever(ServerN) == 0)		//����ǰL�ĸ��ؽڵ㲻����
		return 0;
	if (send(accept_socket[ServerN],Name,20,0)==-1)
	{
		if (send(accept_socket[0],"DS",20,0)==-1)//��L���Ľ�����������Խڵ�
		{
			AgainConnect(10000 );				//�����ӻ���
			return -1;
		}
	}
	if ((chcnt=recv(accept_socket[ServerN],Name,20,0))==-1)	//ʹ��accept�������ص�socket��!
	{
		if (send(accept_socket[0],"DS",20,0)==-1)//��R��ѯ���Ľ�����������Խڵ�
		{
			AgainConnect(10000 );				//�����ӻ���
			return -1;
		}
	}
	P.N[ServerN].Num = 0;
	TensionMin( ); //���ؾ���
	Name[chcnt]='\0';
	if (send(accept_socket[0],Name,20,0)==-1)//��R��ѯ���Ľ�����������Խڵ�
	{
		AgainConnect(10000 );				//�����ӻ���
		return -1;
	}
	remove(SeverBack);//ɾ���ļ�
	cleanRB();		  //ɾ���ڴ���
	cleanStack();	  //ɾ��ջ
}

int printfNum( )         //���ı����������ɽڵ�ĸ������Լ��ܵĸ��ɽڵ�������ӡ
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 10);//����printf��ɫ
	printf("\t\tPlNodeA:%d",(P.N[1].Num == MAX)?-1:P.N[1].Num);
	printf("\tPlNodeB:%d",(P.N[2].Num == MAX)?-1:P.N[2].Num);
	printf("\tPlNodeC:%d",(P.N[3].Num == MAX)?-1:P.N[3].Num);
	printf("\n\t\tPlNodeD:%d",(P.N[4].Num == MAX)?-1:P.N[4].Num);
	printf("\tPlNodeE:%d\n",(P.N[5].Num == MAX)?-1:P.N[5].Num);
	printf("\t\tAllPlNode:%d\n",P.Total);
	return 0;
}