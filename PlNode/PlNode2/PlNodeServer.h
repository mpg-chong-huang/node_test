#include "RBTree.h"

/************************************************************************/
//IDջ�Ķ���
typedef struct rec_ID_stack{
    int ID;																	//���յ�ID��Ϣ
    struct rec_ID_stack   *next;											//ָ����
	int blank[8];															//���ֵ�ַ����
}rec_ID_stack;

/************************************************************************/
//ȫ�ֱ�������
int WinSocketClient;														//�����׽��ֱ���.
int WinSocketClient1;												        //�����׽��ֱ���.
struct sockaddr_in ServerAddr,ClientAddr;									//�����ַ�ṹ����.				
LPCSTR ip;																	//����IP��ַ
HANDLE consolehwnd;															//������ɫ��� 
char resultMes[10] = "-1";													//���ظ�Ctr�ڵ����Ϣ
RBTree tree[62];															//�����ڵ�
int sum_id=0;																//ȫ�ֱ���sum_id����ʾ�ѷ����û�id����
int exist_sum_id= 0;														//��ʾ�ִ��id������
																			//��Ϊ��ɾ�����û����ID������գ�																			//����sum_id�ܴ��ڻ����exist_sum_id,ͬʱ��ʾ�û���
rec_ID_stack * top;															//ջ��ָ��
int isConnect = 0;

/************************************************************************/
//��������
int socketInit();															//socket��ʼ������
void socketGetData( int );													//�������ݴ�����
void getInfos( char * );													//������CtrPlNode����������
DWORD WINAPI server( LPVOID );												//�߳�server,ģ��Ϊÿ���û��ṩ�ķ���
void push( int );															//��ɾ���û���ID��������ջ
void pop();																	//ջ�����û���Ϣ��ջ
int check_SameName( int,char * );											//���������û����Ƿ��������Ƿ���1���񷵻�0
void Insert( char *, int );													//����������û���Ϣ
int Delete( char *, int );													//ɾ���û���㡣��ɾ�����û����ID�������
int RecUserCmd( char *,int );												//����RecUserCmdʵ�������û���Ϣ��ɾ���û���Ϣ�Ĺ���
int getHash( char );														//�õ���ϣֵ
DWORD WINAPI HR( LPVOID );													//�����̺߳���

/************************************************************************/
//��ջ
void push( int id)															//��ɾ���û���ID��������ջ
{	   		  		       
    rec_ID_stack * r;
	r = (rec_ID_stack *) MemoryPool_alloc();
	r->ID=id;																//����id�����û�		
    r->next = top->next; 
    top->next = r;  

 }

/************************************************************************/
//��ջ
void pop()																	//ջ�����û���Ϣ��ջ
{
	rec_ID_stack * p;
	p = top ->next;
	top->next = top->next->next;	
	Memory_recover(p);
}

/************************************************************************/
//��ѯ
int check_SameName(int Hash,char *input)									//���������û����Ƿ��������Ƿ���1���񷵻�0
{	
	PRBTNode p = NULL;
	input = &input[1];
	p = searchTNode(&tree[Hash],input);

	if( 0 == strcmp(p->Name,input))
		return 1;
		
	return 0;
}

/************************************************************************/
//����
void Insert( char * input, int Hash)										//����������û���Ϣ
{	   		  		       
		int ID;
		PRBTNode p = NULL;
		if (top->next != NULL)
		{
            ID=top->next->ID;
			pop();		
		}
        else{
			ID = sum_id+1;													//����id�����û�
			sum_id++;														//�ۼ��û�id���� 
		}

		p = insertRB(&tree[Hash],input,ID);
		exist_sum_id++;														//����Ŀǰ�����û�����

		SetConsoleTextAttribute(consolehwnd, 10);							//��������Ϊ��ɫ
		printf("�������û�:%s,ID:%d",p->Name,p->ID); 
		SetConsoleTextAttribute(consolehwnd, 11);							//��������Ϊ��ɫ
		printf("\n���������û�������Ϊ��%d\n\n",exist_sum_id);
	
		//�������߳�Ϊ��ǰ�û�����
		CreateThread( NULL, 0, server, NULL, 0, 0 );
		
 }

/************************************************************************/
//ɾ��
int Delete(char * input, int Hash)											//ɾ���û���㡣��ɾ�����û����ID�������
{
		PRBTNode p = NULL;
		p = searchTNode(&tree[Hash],input);
		//���ҳɹ�
		if( 0 == strcmp(p->Name,input) )
		{	 
			 push(p->ID);

			 p = deleteRB(&tree[Hash],p);

			 SetConsoleTextAttribute(consolehwnd, 12);						//��������Ϊ��ɫ
			 printf("ɾ���û�:%s,ID:%d",p->Name,p->ID); 
		
			 Memory_recover(p);
			 exist_sum_id--;

			 SetConsoleTextAttribute(consolehwnd, 11);						//��������Ϊ��ɫ
			 printf("\n���������û�������Ϊ��%d\n\n",exist_sum_id);
			 
			 return 0;														//����0��ʾɾ���û����ɹ���
		}	
		
			return 1;														//����1��ʾҪɾ�����û�������
	
}


/************************************************************************/
//�����û�����
int RecUserCmd(char * input,int PlNode)										//����RecUserCmdʵ�������û���Ϣ��ɾ���û���Ϣ�Ĺ���
{
	char str1[20] = {'\0'};
	int  f;
	int Hash_Now = 0;
	char c;
	PRBTNode p=NULL;

		c = input[1];														//�õ��û����ĵ�һ���ַ�
		Hash_Now = getHash(c);												//ͨ��hash�����õ������±�

		if ('+' == input[0])												//ʵ�������û���Ϣ����
		{					 
			 input = &input[1];
			 Insert(input, Hash_Now);										//���ú���Insert_S�������û����	 
			 itoa(exist_sum_id,str1,10);
			 strcpy(resultMes,"1,");
			 strcat(resultMes,str1);
			 return 1;
		}
		else if('~' == input[0])											//ɾ���û�
		{

			input = &input[1];
			f = Delete(input, Hash_Now);									//���ú���Delete_Sɾ������û�
			
			if (1 == f)														//���fֵΪ1����˵��Ҫɾ�����û���㲻����
			{
				itoa(exist_sum_id,str1,10);
			    strcpy(resultMes,"3,");
				strcat(resultMes,str1);
				return 1;
			}
			 itoa(exist_sum_id,str1,10);
			 strcpy(resultMes,"1,");
			 strcat(resultMes,str1);
			return 1;
		}
		else if('#' == input[0])											//��ѯ�û��Ƿ��ڸ÷�������
		{
			if( 1 == check_SameName(Hash_Now,input) )
			{
				itoa(PlNode,str1,10);
				strcpy(resultMes,str1);
			}
			else
			{
				strcpy(resultMes,"0");
			}
			return 1;
		}
		else if('@' == input[0])											//��ѯ����������
		{
			itoa(exist_sum_id,resultMes,10);
			  return 1;
		}
		else if('_' == input[0])											//�����Ƿ�����
		{
			f=check_SameName(Hash_Now,input);
			if (1 == f)
			{
				strcpy(resultMes,"2");
				SetConsoleTextAttribute(consolehwnd, 12);						//��������Ϊ��ɫ
				return 1;
			}
			
			strcpy(resultMes,"1");
			return 1;
		}
		else if('R' == input[0])											//�ͷ������û�
		{	
			for(Hash_Now = 0;Hash_Now<62;Hash_Now++)						//����61����
			{
				while(tree[Hash_Now].root!=tree[Hash_Now].nil)				//����ͷ����ָ���ڱ��ڵ�
				{
					printf("�ͷ��û�%s,ID:%d\n",tree[Hash_Now].root->Name,tree[Hash_Now].root->ID); 
					push(tree[Hash_Now].root->ID);							//����ID
					exist_sum_id--;
					p = deleteRB(&tree[Hash_Now],tree[Hash_Now].root);
					Memory_recover(p);
				}
			}

			if(exist_sum_id == 0)
			{
				strcpy(resultMes,"1");                                      //�ͷųɹ�
			}
			else
			{
				strcpy(resultMes,"2");										//�ͷ�ʧ��
			}
			SetConsoleTextAttribute(consolehwnd, 11);						//��������Ϊ��ɫ
			printf("���ͷ������û�,��ǰ�û���Ϊ��%d\n",exist_sum_id);
			return 1;
		}
		else if ('S' == input[0])
		{
			exit(1);
		}

	
		return 0;
}



/************************************************************************/
//socket��ʼ������
int socketInit()
{
	char name[255];															//���ڴ��������
    PHOSTENT hostinfo;														//������Ϣ���ݽṹ����,���ڴ�ź������ص���������.
	WSADATA wsaData;														//����WSAStartup��������ֵ.
	  
	if ( WSAStartup( MAKEWORD( 2, 2 ), &wsaData ) == 0 )
	{
		//����WSAStartup�����Ǽ���Winsock.dll�⣬
		//���WSAStartup()�����ķ���ֵΪ0��˵�����سɹ�
		//��ȡ������IP��ַ,�Ա��������ʹ��.
		if( gethostname ( name, sizeof(name)) == 0) 
		{
			//�ж��Ƿ�ɹ��Ľ������������������name����ָ���Ļ�������
			if((hostinfo = gethostbyname(name)) != NULL) //��ȡ��������Ϣ.
			{
				//�������������ɹ��Ļ�������inet_ntoa()����ȡ��IP��ַ

				ip= inet_ntoa (*(struct in_addr *)*hostinfo->h_addr_list);
				
			} 
		}
		
	//���������IP��ַ�ڱ���ip��. 
	//=============================================
    return 1; 
	}
	return 0;
}

void pre4Connect(int *Client,int port)
{
	u_short dport=port;
	consolehwnd = GetStdHandle(STD_OUTPUT_HANDLE);							//ʵ�������
	ClientAddr.sin_addr.S_un.S_addr= inet_addr(ip);							//���ñ�����IP��ַ.
	//inet_addr()���صĵ�ַ�Ѿ��������ֽڸ�ʽ�������������ٵ��� ����htonl()��
	
	ClientAddr.sin_family=AF_INET;											//ָ��ΪTCP/IP��ַ����
	
	memset(&ClientAddr.sin_zero,8,0);	
	
	ServerAddr.sin_addr.S_un.S_addr= inet_addr(ip); 
	
	ServerAddr.sin_port=htons(dport);          								//�˿ں�,htons()������ʽת��Ϊ�����ʽ
		
	ServerAddr.sin_family=AF_INET;
	
	memset(&ServerAddr.sin_zero,8,0);
	
	//������� socket.
	if ((*Client=socket(AF_INET,SOCK_STREAM,0))==-1)	
		//AF_INETΪTCP/IPЭ���V4�汾
		//SOCK_STREAMΪ�ֽ������(stream socket),ʵΪTCPЭ��.
		//0 ��ΪTYPE�Ѿ�ָ����TCPЭ��,����Э��������0,����ʹ��Ĭ��Э��.
		{
		SetConsoleTextAttribute(consolehwnd, 12);							//��������Ϊ��ɫ
		printf("����SOCKETʧ��!!!\n");
		
		}
}

/************************************************************************/
//����ת���ڵ�
void Connect2CtrlNode(int Client,int k)
{
	struct sockaddr SA1;
	int i = 1;
	while(1)
	{	Sleep(85);
		if (connect(Client,(struct sockaddr *)&ServerAddr,sizeof(SA1))==-1)
		{
			SetConsoleTextAttribute(consolehwnd, 12);						//��������Ϊ��ɫ
			printf("\n�����ܿؽڵ�ʧ��!!!\n��");
				
			SetConsoleTextAttribute(consolehwnd, 11);						//��������Ϊ��ɫ
			printf("%d",i);
			SetConsoleTextAttribute(consolehwnd, 12);						//��������Ϊ��ɫ

			printf("������...\n\n");
			i ++;
			continue;
		}		
		i = 1;
			break;
	}
	SetConsoleTextAttribute(consolehwnd, 10);								//��������Ϊ��ɫ
	if(k)
	{
		printf("�ܿؽڵ����ӳɹ�!\n\n");	
		itoa(exist_sum_id,resultMes,10);
		if (send(Client,resultMes,strlen(resultMes),0)==-1)					//��������,���ظ�����������Ϣ
		{
			SetConsoleTextAttribute(consolehwnd, 12);						//��������Ϊ��ɫ
			printf("����ʧ��!\n");
		}
	}
	else
	{
		printf("���ܿؽڵ㽨���������!\n\n",ip);
		isConnect =1;

	}
}

/************************************************************************/
//socketͨ�ź���
void socketGetData(int PlNode)
{
	
    char InputStr[256];														//�����ֽڵĻ�����
    int sendchrcnt;															//���յ����ֽ�������
	
	while(1)
	{
		
			if ((sendchrcnt=recv(WinSocketClient,InputStr,20,0))==-1)
			{
				continue;
			}
			else
			{
				InputStr[sendchrcnt]='\0';
				while (0 ==isConnect )
				{
					Sleep(1000);
				}
				if(	!RecUserCmd(InputStr,PlNode) )
				{
					printf("���յ�������%s,ϵͳ�޷�����!",InputStr);
					getchar();
				}
			}

       
			if (send(WinSocketClient,resultMes,strlen(resultMes),0)==-1)	//��������,���ط���������Ϣ
			{
				continue;
			}
			strcpy(resultMes,"-1");											//ÿ�η����������
			
		Sleep(10);
	} 

		closesocket(WinSocketClient);
		WSACleanup( );														//ж��Winsock�⣬���ͷ�������Դ
	}

/************************************************************************/
//�ṩ����
DWORD WINAPI server( LPVOID lpParam )
{
	//Ϊ�û��ṩĳ�����
	 Sleep(2000);

			return 0;
}

/************************************************************************/
//hash����                                                             
int getHash(char c)
{                                           
	if(c>='0' && c<='9' )                 //��ϣֵΪ0��9
	{
		return (c - '0');
	}
	else if(c>='A' && c<='Z')             //��ϣֵΪ10��35
	{
		return (c - 55);
	}
	else if(c>='a' && c<='z')             //��ϣֵΪ36��61
	{
		return (c - 61);
	}
		return -1;
}



/************************************************************************/
//���������߳�
DWORD WINAPI HR( LPVOID lpParam )
{
	int dport = *(int*)(lpParam);

	//����ǰ��׼��
	pre4Connect(&WinSocketClient1,dport+10);
	//����������
	Connect2CtrlNode(WinSocketClient1,0);	

	while(1)
	{
		if (send(WinSocketClient1,"HR",2,0)==-1)
		{
			isConnect =0;
			SetConsoleTextAttribute(consolehwnd, 12);					//��������Ϊ��ɫ
			printf("\n����������ʧ��!!!\n");

			//�շ��˿�����
			pre4Connect(&WinSocketClient,dport);
			Connect2CtrlNode(WinSocketClient,1);	

			//�����˿�����
			pre4Connect(&WinSocketClient1,dport+10);
			Connect2CtrlNode(WinSocketClient1,0);
			continue;
		}
		Sleep(20);
	}
	
	return 0;
}