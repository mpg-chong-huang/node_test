#include "head.h"

#define INITPOOL 5000				//ÿ�����ڴ�صĳ�ʼ��С
#define ADDPOOL 5000			    //ÿ���������ڴ�صĳ�ʼ��С
#define Byte 44						//ÿ���·����ڴ��ֽ���

typedef struct Memory_Block{					   //�û���Ϣ�ṹ��
	char color;                                    //�������ɫ��
    struct Memory_Block * p;                       //ָ�򸸽ڵ�ָ��
    struct Memory_Block * left;                    //ָ������ָ��
    struct Memory_Block * right;                   //ָ���Һ���ָ��
	
	int ID;                                        //������
	char Name[20];
	struct Memory_Block   *freeNext;	//free����nextֵ
}MemoryBlock;

typedef struct Memory_Pool{				//�ڴ��
	MemoryBlock   *Init_Pool;			//�ص��ڴ�����
    MemoryBlock   *free;				//�ڴ���п�����������
	int freeNum;						//���п�����
	struct Memory_Pool *PoolNext;		//�����ڴ�ص�����
}MemoryPool;

MemoryPool Memory;						//�ڴ��

void MemoryPool_init(MemoryPool *Mem )	//��ʼ���ڴ�غ�������һ�ε���ʱ���ʼ��Memory
{
	int i = 0;							//����ص��ڴ�ռ�
	Mem->Init_Pool = (MemoryBlock *)malloc(INITPOOL * sizeof( MemoryBlock ));
	Mem->freeNum = INITPOOL;			//���д�С
	Mem->PoolNext = NULL;				//��һ���ڴ��ָ��
	Mem->free = &(Mem->Init_Pool[0]);	//��������ͷָ��
	for (i=1; i<INITPOOL; i++)			//�齨free����
	{
		(Mem->Init_Pool[i-1]).freeNext = &(Mem->Init_Pool[i]);
	}
	Mem->Init_Pool[INITPOOL-1].freeNext = NULL;//��Init_Pool���鴮��free����
}
void * MemoryPool_alloc( )				//�����ڴ�"����(malloc)����"
{										//ʹ�÷�����MemoryBlock *XX = (MemoryBlock *)MemoryPool_alloc( );��
	MemoryPool *Temp = &Memory,*Bew;
	MemoryPool *AddPool;				//�����ڴ��
	MemoryBlock *Return;				//���ڷ������뵽���ڴ�ռ��ַ��
	while (Temp->freeNum == 0)			//��ǰ�ڴ��free�ڴ�ռ�Ϊ0ʱ
	{					
		Bew = Temp;
		Temp = Temp->PoolNext;
		if (NULL == Temp)				//˵����ǰ���е��ڴ�ز�������
		{								//�²������ڴ�����ڷ���
			AddPool = ( MemoryPool * )malloc( sizeof( struct Memory_Pool )*1 );
			if (NULL == AddPool)
			{
					printf("Can`t get memory for that many value.\n" );
					return NULL;		//����ʧ�ܷ���NULL
			}
			MemoryPool_init(AddPool);	//��ʼ���²������ڴ��
			Temp = AddPool;
			Bew->PoolNext = Temp;		//���²������ڴ�ز��뵽�ڴ��������
		}
	}
	Return = Temp->free;				//��÷��ط��䵽���ڴ��ַ
	Temp->free = (Temp->free)->freeNext;
	Temp->freeNum--;					//�ع�free����
	return Return;
}
void Memory_recover(MemoryBlock *ptr)	//�����ڴ�"����(free)����" ʵ���ǽ����սڵ���free������
{										//ʹ�÷�����Memory_recover(XX);��
	MemoryPool *Temp = &Memory,*Bew;
	MemoryBlock *ReFree;
	int num = 0;						
	while(NULL != Temp)					
	{
		ReFree = Temp->free;			
		num = (ptr - &(Temp->Init_Pool[0]));//ͨ���ж��ڴ��ַ��λ�����ڴ��������һ���ڴ����
		if (num<INITPOOL && num >= 0)	//�ҵ��ڴ��
		{	
			Temp->free = ptr;
			Temp->free->freeNext=ReFree;//�������յ��ڴ����·���free������
			Temp->freeNum++;
			if (INITPOOL == Temp->freeNum && &Memory != Temp)
			{							//��ʱ˵����ǰ�ڴ���ѿ�
				Bew->PoolNext = Temp->PoolNext;	
				free(Temp->Init_Pool);
				free(Temp);				//�������ڴ�ؿռ�free������Ϊ��������ڴ汻free
			}							//�ʲ������������볷���ڴ�ʱ���������ڴ���Ƭ����
			return ;
		}
		Bew = Temp;
		Temp = Temp->PoolNext;			//������֯�ڴ������
	}
}
