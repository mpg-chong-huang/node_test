#include "head.h"

#define INITPOOL 5000				//每个的内存池的初始大小
#define ADDPOOL 5000			    //每个新增的内存池的初始大小
#define Byte 44						//每个新分配内存字节数

typedef struct Memory_Block{					   //用户信息结构体
	char color;                                    //红黑树颜色域
    struct Memory_Block * p;                       //指向父节点指针
    struct Memory_Block * left;                    //指向左孩子指针
    struct Memory_Block * right;                   //指向右孩子指针
	
	int ID;                                        //数据域
	char Name[20];
	struct Memory_Block   *freeNext;	//free链表next值
}MemoryBlock;

typedef struct Memory_Pool{				//内存池
	MemoryBlock   *Init_Pool;			//池的内存数据
    MemoryBlock   *free;				//内存池中空闲数据链表
	int freeNum;						//空闲块数量
	struct Memory_Pool *PoolNext;		//串接内存池的链表
}MemoryPool;

MemoryPool Memory;						//内存池

void MemoryPool_init(MemoryPool *Mem )	//初始化内存池函数，第一次调用时需初始化Memory
{
	int i = 0;							//分配池的内存空间
	Mem->Init_Pool = (MemoryBlock *)malloc(INITPOOL * sizeof( MemoryBlock ));
	Mem->freeNum = INITPOOL;			//空闲大小
	Mem->PoolNext = NULL;				//下一个内存池指针
	Mem->free = &(Mem->Init_Pool[0]);	//空闲链表头指针
	for (i=1; i<INITPOOL; i++)			//组建free链表
	{
		(Mem->Init_Pool[i-1]).freeNext = &(Mem->Init_Pool[i]);
	}
	Mem->Init_Pool[INITPOOL-1].freeNext = NULL;//将Init_Pool数组串成free链表
}
void * MemoryPool_alloc( )				//申请内存"类似(malloc)作用"
{										//使用方法“MemoryBlock *XX = (MemoryBlock *)MemoryPool_alloc( );”
	MemoryPool *Temp = &Memory,*Bew;
	MemoryPool *AddPool;				//新增内存池
	MemoryBlock *Return;				//用于返回申请到的内存空间地址，
	while (Temp->freeNum == 0)			//当前内存池free内存空间为0时
	{					
		Bew = Temp;
		Temp = Temp->PoolNext;
		if (NULL == Temp)				//说明当前已有的内存池不够分配
		{								//新产生的内存池用于分配
			AddPool = ( MemoryPool * )malloc( sizeof( struct Memory_Pool )*1 );
			if (NULL == AddPool)
			{
					printf("Can`t get memory for that many value.\n" );
					return NULL;		//分配失败返回NULL
			}
			MemoryPool_init(AddPool);	//初始化新产生的内存池
			Temp = AddPool;
			Bew->PoolNext = Temp;		//将新产生的内存池插入到内存池链表上
		}
	}
	Return = Temp->free;				//获得返回分配到的内存地址
	Temp->free = (Temp->free)->freeNext;
	Temp->freeNum--;					//重构free链表
	return Return;
}
void Memory_recover(MemoryBlock *ptr)	//回收内存"类似(free)作用" 实际是将回收节点插进free链表中
{										//使用方法“Memory_recover(XX);”
	MemoryPool *Temp = &Memory,*Bew;
	MemoryBlock *ReFree;
	int num = 0;						
	while(NULL != Temp)					
	{
		ReFree = Temp->free;			
		num = (ptr - &(Temp->Init_Pool[0]));//通过判断内存地址定位回收内存存在于哪一个内存池中
		if (num<INITPOOL && num >= 0)	//找到内存池
		{	
			Temp->free = ptr;
			Temp->free->freeNext=ReFree;//将欲回收的内存重新放入free链表中
			Temp->freeNum++;
			if (INITPOOL == Temp->freeNum && &Memory != Temp)
			{							//此时说明当前内存池已空
				Bew->PoolNext = Temp->PoolNext;	
				free(Temp->Init_Pool);
				free(Temp);				//将整个内存池空间free掉，因为大快连续内存被free
			}							//故不会像零碎申请撤销内存时那样产生内存碎片问题
			return ;
		}
		Bew = Temp;
		Temp = Temp->PoolNext;			//重新组织内存池链表
	}
}
