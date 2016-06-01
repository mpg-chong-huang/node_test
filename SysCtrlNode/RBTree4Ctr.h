#include "RBTree.h"
/************************************************************************/
//IDջ�Ķ���
typedef struct rec_ID_stack{
    int ID;																	//���յ�ID��Ϣ
    struct rec_ID_stack   *next;											//ָ����
	int blank[8];															//���ֵ�ַ����
}rec_ID_stack;

/************************************************************************/
//���ؽڵ��ʶ����
typedef struct queue{
	int n ;
	struct queue *next;
}queue;


RBTree tree[6];																//�����ڵ�
const long P_SIZE = 24;														//�ļ���ÿ���ڵ�Ĵ洢���ȣ�20�ֽ��ַ����飬4�ֽ�long
const char nul[30] = "****************************\n" ;                     //ɾ�����
int sum_id= 0;																//ȫ�ֱ���sum_id����ʾ�ѷ����û�id����
int exist_sum_id= 0;														//��ʾ�ִ��id������
rec_ID_stack * top;															//ջ��ָ��
queue * QueTail, *QueHead;                                                  //�����ͷ��βָ��


//���г�ʼ��
void initQue()
{
	QueHead = (queue *) MemoryPool_alloc();
	QueTail = QueHead;
	QueHead->n = QueTail->n = -1;
}

//���
void inQueue(int serverNum)
{
     queue *q;
	 q = (queue*)MemoryPool_alloc();
	 q->n = serverNum;
	 if (QueHead == QueTail )												//����ǵ�һ��Ԫ�����
	 {
		 QueHead->next = q;													//��ͷ����һ���ڵ�ָ��q										
		 QueTail = q;														//��βָ��q
	 }																		
	 QueTail->next = q;														//��β����һ���ڵ�ָ��q
	 QueTail = q;															//��βָ��q
}

//����
void outQueue(int *n)
{
	queue *q;
	q = QueHead->next;														//qָ����еĵ�һ��Ԫ��
	*n = q->n;																//�ѷ�������ʶֵȡ��
	if (q->next!=QueTail->next)												//�Ƿ񵽶�β
	{
		QueHead->next = q->next;											//û�е���β
	}
	else
	{
		QueTail = QueHead;													//����β
	}
	Memory_recover(q);
	
}

/************************************************************************/
//��ջ
void push( int id,rec_ID_stack *t)															//��ɾ���û���ID��������ջ
{	   		  		       
    rec_ID_stack * r;
	r = (rec_ID_stack *) MemoryPool_alloc();
	r->ID=id;																				//����id�����û�		
    r->next = t->next; 
    t->next = r;  

}

/************************************************************************/
//��ջ
void pop(rec_ID_stack *t)																	//ջ�����û���Ϣ��ջ
{
	rec_ID_stack * p;
	p = t->next;
	t->next = t->next->next;	
	Memory_recover(p);
}


/********************************************************/
//n�ú������ʼ��,nС�ڵ���5
void initRBS()
{
	int i;
	for (i = 0; i<6;i++)
	{
		initRB(&tree[i]);                       //������ĳ�ʼ��
	}
}

void initIDStack()
{
	//ջ���ڵ�ĳ�ʼ��
		top = (rec_ID_stack *) malloc(sizeof(rec_ID_stack));
		top->next = NULL;

}

/********************************************************/
//���ļ�,�ɹ�����1,ʧ�ܷ���0
FILE *openFile(const char *fName,FILE **fpp)
{
	while(1)
	{
		
	
		if( (*fpp = fopen(fName,"r+")) == NULL )
		{
			if( (*fpp = fopen(fName,"a+")) == NULL )
			{
				printf("�����ļ�ʧ��,ϵͳ�����´���...");
				continue;
			}
			else
			{
				fclose(*fpp);
			}
		
		}
		else
			break;
	}
	return *fpp;
}

/******************************************************/
//�ر��ļ�
void closeFile(FILE *fpp)
{
	fclose(fpp);
}

/*******************************************************/
//д�ļ�,�ɹ�����1,ʧ�ܷ���0
int write2File(FILE *fpp,char *input,int id)
{
	clearerr(fpp);
	fseek(fpp,30L*id,0);
	//fputs�ɹ�����0
	if(  !fputs(input,fpp) )
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

/*******************************************************/
//���ļ�,��ȡ��Ϸ���1,δ�귵��0,���ļ������е��û�������str
int readFromFile(FILE *fpp,char *str)
{
	char *p;
	p = str;
	if( fgets(str+1,30,fpp) )	
	{
		if (str[1] == '*')
		{
			push(sum_id,top);
			sum_id++;
		}
		//�õ��û���
		while(p[0] != '*')
		{
			p++;
		}
		p[0] = '\0';

		if(str[1] != '\0')
		{	
			sum_id++;
			exist_sum_id++;
			
		}
		//δ��ȡ��
		return 0;
	}
	else
	{
		//��������ļ��Ķ�ȡ
		return 1;
	}

}

/*****************************************************/
//ɾ���ļ���ĳ�ڵ�,�ɹ�����1,ʧ�ܷ���0
int delFnode(FILE *fpp, int id)
{
	//�ڵ㶨λ
	fseek(fpp,30L*id,0);
	fputs(nul,fpp);
	return 1;
}

/*****************************************************/
//��ʽ���ַ���
char * formatStr(char *s)
{
	int i=0,n;
	n = strlen(s);
	for(i = n;i<28;i++)
	{
		s[i] = '*';
	}
	s[28] = '\n';
	s[29] = '\0';
	return s;
}

/*************************************************/
//��ת���ڵ�ֱ�ӵ��õĺ������ܹ����ڴ���ɾ��������ڵ㣬���Ѹýڵ������Ϣ���ļ���ɾ��
int delNode(char *userN,char *fName,int k)
{
		PRBTNode p = NULL;
		FILE *fp;
		p = searchTNode(&tree[k],userN);
		//���ҳɹ�
		if( 0 == strcmp(p->Name,userN) )
		{	 
			push(p->ID,top);
			if(!openFile(fName,&fp))
			{//ʧ��
				 return 0 ;
			}
			else
			{//�ɹ�
				delFnode(fp,p->ID);
				closeFile(fp);
			}
			 p = deleteRB(&tree[k],p);	
			 Memory_recover(p);
			 exist_sum_id--;
			 return 1;
		}	
		else
		{
			return 0;
		}
}

/***************************************/
//����ڴ��еĺ����
int cleanRB()
{
	int i =1;
	PRBTNode p=NULL;
	while(tree[i].root!=tree[i].nil)				//����ͷ����ָ���ڱ��ڵ�
	{
		p = deleteRB(&tree[i],tree[i].root);
		Memory_recover(p);
		i++;
	}
	return 1;
}

/********************************************/
//���ID
int cleanStack()
{
	while(top->next != NULL)
	{
		pop(top);	
	}
	sum_id = 0;
	exist_sum_id = 0;
	return 1;

}

/*******************************************************/
//�������
int cleanQue()
{
	int n;
	while (QueHead != QueTail)
	{
		outQueue(&n);
	}
	return 1;
}
/********************************************************/
//���г�ʼ��
void allinit()
{
	initRBS();
	initIDStack();
	initQue();
	MemoryPool_init(&Memory);
}


/*******************************************************/
//����ڴ棬IDջ��ɾ���ļ�
void allClean(char *fName)
{
	cleanRB();
	cleanStack();
	cleanQue();
	remove(fName);
}


/*******************************************************/
//д��ĳ�ú�����ڴ�,����id,��IDΪ-1����Ҫ��̬����ID,������Ҫ
int write2RBTree(char *userN,int k,int ID)
{
	//���������  		       
	int id = 0;
	PRBTNode p = NULL;

	if(-1 == ID)
	{//��̬����
		if (top->next != NULL)
		{
			id=top->next->ID;
			pop(top);	
		}
		else{
			id = sum_id;													//����id�����û�
			sum_id++;														//�ۼ��û�id���� 
		}

	}
	else
	{//ֱ��ȡID
		id = ID;
	}

	p = insertRB(&tree[k],userN,id);
	exist_sum_id++;

	return p->ID;

}


/*************************************************/
//��ת���ڵ�ֱ�ӵ��õĺ������ѽڵ㱸�ݵ��ļ���,�ɹ�����1��ʧ�ܷ���0
int backup2File(char *userN,char *fName,int id)
{
	char str1[30];
	char *s;
	FILE *fp;
	char temp[10];
	//д���ļ�
	//����Ҫ��ʽ��д���ļ����ַ���
	//1��30���ַ���30���ֽ�
	//2����ʽ���գ�"�û�����ID****..."������'*'�����
	if(!openFile(fName,&fp))
	{//ʧ��
		
		return 0 ;
	}
	else
	{//�ɹ�
		
		strcpy(str1,userN);
		s = formatStr(str1);
		fseek(fp,0L,0);
		write2File(fp,s,id);
		closeFile(fp);
		return 1;
	}
}

/*******************************************************/
//����ʱ���е���ͷת������k
void changeTree(char *uName,int k,int ID)
{
	insertRB(&tree[k],uName,ID);
}


/******************************************************/
//����ʱ��
void tempTree(int n)
{
	tree[0].nil = tree[n].nil;
	tree[0].root = tree[n].root;
	tree[0].root->color = tree[n].root->color;
	tree[0].root->freeNext = tree[n].root->freeNext;
	tree[0].root->ID = tree[n].root->ID;
	tree[0].root->left = tree[n].root->left;
	strcpy(tree[0].root->Name,tree[n].root->Name);
	tree[0].root->parent = tree[n].root->parent;
	tree[0].root->right = tree[n].root->right;
}


/*****************************************************/
//д��־�ļ�
int write2Log(char *Node,char *fName,int isConnect)
{
	FILE *fp;
	SYSTEMTIME time;
	TCHAR strTime[256];
	GetLocalTime(&time);
	wsprintf(strTime,"%04d-%02d-%02d %02d:%02d:%02d ",time.wYear,time.wMonth,time.wDay,time.wHour,time.wMinute,time.wSecond);	
	strcat(strTime,Node);

	if( isConnect == 1 )
	{
		strcat(strTime,"����\n");
	}
	else
	{
		strcat(strTime,"�ر�\n");
	}


	if( (fp = fopen(fName,"a+")) == NULL )
	{
		printf("������־�ļ�ʧ��!");
	}
	else
	{
		fputs(strTime,fp); 
		fclose(fp);
	}
	return 0;
}