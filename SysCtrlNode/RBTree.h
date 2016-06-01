#include "MemoryPool.h"

/************************************************************************/
#define RED    0
#define BLACK  1

/************************************************************************/
//红黑树节点
typedef struct RBTNode{ 
	char color;																	//红黑树颜色域
    struct RBTNode * parent;													//指向父节点指针
    struct RBTNode * left;														//指向左孩子指针
    struct RBTNode * right;														//指向右孩子指针
	
		int ID;																		//数据域
	char Name[20];
	struct RBTNode   *freeNext;													//内存池free链表next值
}RBTNode, * PRBTNode;

/************************************************************************/
//统一节点
typedef struct RBTree{    
	PRBTNode root;																//根结点，该节点为黑色
	PRBTNode nil;																//统一的空节点，该节点是黑的
}RBTree, * PRBTree;


/************************************************************************/
//关于红黑树的函数定义
int leftRotate (PRBTree tree, PRBTNode t);										//树的左旋
int rightRotate (PRBTree tree, PRBTNode t);										//树的右旋
PRBTNode insertRB (PRBTree tree, char* input,int ID);							//插入
int insertRB_fixup (PRBTree tree, PRBTNode t);									//为插入保持红黑性质
PRBTNode deleteRB (PRBTree tree, PRBTNode t);									//删除
int deleteRB_fixup (PRBTree tree, PRBTNode c);									//为删除保持红黑性质
PRBTNode minimum (PRBTree tree, PRBTNode t);									//找到以节点t为树根的子树的最小节点
PRBTNode next (PRBTree tree, PRBTNode t);										//找到t节点的后继
PRBTNode searchTNode(PRBTree tree, char *input);								//在树中查找与关键字相同的节点

/************************************************************************/
//左旋，不改变中根遍历顺序
int leftRotate (PRBTree tree, PRBTNode t)
{    PRBTNode c;																//左旋   
	 c = t->right;    

	 if (t->right == tree->nil)													       
		 return 1;                       
	 
	 
	   
	 if (t->parent != tree->nil)												//判断t是否为根节点    
	 {        
		 if (t->parent->left == t)												//判断x是x的父节点的左孩子还是右孩子            
			 t->parent->left = c;          
		 else            
			 t->parent->right = c;    
	 }    
	 else																		//x是根节点
		 tree->root = c;    

	 c->parent = t->parent;														//更新y的父节点   
	 
	 t->right = c->left;                 
	 if (c->left != tree->nil)        
		 c->left->parent = t;    

	 c->left = t;    
	 t->parent = c;    
	 return 0;
}

/************************************************************************/
//树的右旋，不改变中根遍历顺序
int rightRotate (PRBTree tree, PRBTNode t)
{    
	 PRBTNode c;																//右旋   
	 c = t->left;    

	 if (t->left == tree->nil)													       
		 return 1;                       
	 
	 
	 //这个if-else用于将x的父亲节点的lefx或righx点指向y，如果x的父节点为不存在，则树的roox指向y    
	 if (t->parent != tree->nil)												//判断x是否为根节点    
	 {        
		 if (t->parent->left == t)												//判断x是x的父节点的左孩子还是右孩子            
			 t->parent->left = c;          
		 else            
			 t->parent->right = c;    
	 }    
	 else																		//c是根节点
		 tree->root = c;    

	 c->parent = t->parent;														//更新c的父节点  
	 
	 t->left = c->right;                 
	 if (c->right != tree->nil)        
		 c->right->parent = t;    

	 c->right = t;    
	 t->parent = c;    
	 return 0;
}

/************************************************************************/
//红黑树的初始化
int initRB (PRBTree tree)
{    
	if (tree == NULL)        
		return 0;    
	tree->nil = (PRBTNode)malloc(sizeof(RBTNode));    
	tree->nil->color = BLACK;    
	tree->root = tree->nil;    
	return 0;
}//initRB

/************************************************************************/
//插入元素!!!记得插入的元素的初始化，p指向为父母节点，left和right赋值为NULL。  
//该插入操作不允许重名,返回值若是NULL则表明插入失败，有同样的值存在
PRBTNode insertRB (PRBTree tree, char* input,int ID)
{  
	PRBTNode tIn = NULL;													  //指向要插入的节点
	PRBTNode pIn = NULL;                                                      //指向要插入节点地址的父节点
	int flag = 0;															  //用来表示插入在左边的树（0）还是右边的树（1）  
	
	tIn = tree->root;														  //插入的节点是root,并做相应的初始化    
	if (tree->root == tree->nil)    
	{        
        tree->root = (PRBTNode)MemoryPool_alloc( );
		strcpy(tree->root->Name,input);        
		tree->root->color = BLACK;        
		tree->root->ID = ID;
		tree->root->parent = tree->root->left =tree->root->right = tree->nil;               
		return tree->root;    
	}
    
	while (tIn != tree->nil)    
	{        
		pIn = tIn;        
		if (0 > strcmp(input,tIn->Name))        
		{            
			flag = 0;               
			tIn = tIn->left;        
		}        
		else        
		{            
			if ( 0 < strcmp(input,tIn->Name))            
			{                
				flag = 1;                
				tIn = tIn->right;            
			}            
			else  
			{
			if ( (flag=rand()%2) == 0)
                    tIn = tIn->left;
                else
                    tIn = tIn->right;
            }
        }
    }//while
                
    
	//将t指向带插入节点的地址，并初始化      
	tIn = (PRBTNode)MemoryPool_alloc();
	strcpy(tIn->Name,input);    
	tIn->color = RED;    
	tIn->ID = ID;
	tIn->parent = pIn;    
	tIn->left = tIn->right = tree->nil;       
	
	if (!flag)        
		pIn->left = tIn;    
	else        
		pIn->right = tIn;
    
	insertRB_fixup(tree, tIn); 
	
	return tIn;
}

/************************************************************************/
//插入的节点可能破坏红黑树的性质。该函数检测插入的节点是否破坏了红黑树的性质。如果破坏了，就对树进行调整，使其满足红黑树的性质
int insertRB_fixup (PRBTree tree, PRBTNode t)
{    
	//只有插入节点的父亲是红色的才会破坏红黑树的性质（4.如果一个结点是红的，那么它的俩个儿子都是黑的）
	while (t->parent->color == RED)        
	{        if (t->parent->parent->left == t->parent)							//插入节点的父节点本身是left        
			 {            
				if (t->parent->parent->right->color == RED)						//case 1            
				{                                                   
					t = t->parent->parent;                
					t->left->color = t->right->color = BLACK;                
					t->color = RED;            
				}            
				else            
				{                
					if (t->parent->right == t)									//case 2                
					{//将case 2转换为了case 3                            
						t = t->parent;											//这步赋值是为了在转换为case 3时，t指向的是下面的
																				//红节点，和case 3的情况相一致                    
						leftRotate(tree, t);                
					} 
					//case 3                
					t->parent->color = BLACK;                
					t->parent->parent->color = RED;                
					rightRotate(tree, t->parent->parent);            
				}        
			}//if        
			else																//插入节点的父节点本身是right        
			{            
				if (t->parent->parent->left->color == RED)						//case 1            
				{                                                   
					t = t->parent->parent;                
					t->left->color = t->right->color = BLACK;                
					t->color = RED;            
				}            
				else            
				{                
					if (t->parent->left == t)									//case 2                
					{//将case 2转换为了case 3                            
						t = t->parent;											//这步赋值是为了在转换为case 3时，t指向的是下面
																				//的红节点，和case 3的情况相一致                    
						rightRotate(tree, t);                
					}            
					//case 3                
					t->parent->color = BLACK;               
					t->parent->parent->color = RED;    
					leftRotate(tree, t->parent->parent);     
				}    
			}//else   
	}//while    
	tree->root->color = BLACK;    
	return 0;
}


/************************************************************************/
//删除数据。要求给处数据节点的指针
PRBTNode deleteRB (PRBTree tree, PRBTNode t)
{    
	PRBTNode cDele = NULL;														//c指向要取代被删除节点的子节点   
	PRBTNode dDele = NULL;						 
	int tmp;				
	char str1[20]={'\0'};

    if (t == tree->nil)					     
	    return NULL;					         
	                                          
		       
//d指向真正要删除的元素的下标。如果t的left和right都有值，则转化为删除t的后继节点，
//并把后继节点的内容复制给t指向的节点。
//而其他情况则直接删除t指向的节点
	if (t->left != tree->nil && t->right != tree->nil)    
	{      
		dDele = next(tree, t);                     
		tmp = dDele->ID;														//因为实际操作要删除的是d指向的节点，所以先交换数据
		dDele->ID = t->ID;
        t->ID = tmp;  

		strcpy(str1,dDele->Name);
		strcpy(dDele->Name,t->Name);
		strcpy(t->Name,str1);
	}    
	else   
		dDele = t;                                 
	

	if (dDele->left == tree->nil)   
		cDele = dDele->right;													//确定c的指向
	else   
		cDele = dDele->left;                         
    cDele->parent = dDele->parent;												//将c的父亲指针设为d的父亲指针,c不会为空（因为存
																				//在nil节点）

	if (dDele->parent != tree->nil) 
	{      
		if (dDele->parent->left == dDele)  
			dDele->parent->left = cDele;  
		else     
			dDele->parent->right = cDele;    
	}    
	else   
		tree->root = cDele;  
	
	if (dDele->color == BLACK)  
		deleteRB_fixup(tree, cDele);    
	return dDele;
}//deleteRB

/************************************************************************/
//删除辅助函数，保持红黑性质
int deleteRB_fixup (PRBTree tree, PRBTNode c)
{ 
	PRBTNode b;																 //兄弟节点   
	while (c != tree->root && c->color == BLACK)
    {       
		//c是左孩子节点
		if (c == c->parent->left)   
		{   
			b = c->parent->right;  
			//b节点是红的，可以说明c和b的父亲节点是黑的。通过以下的操作可以吧case 1转换为case 2,3,4中的一个 
			if (b->color == RED) //case 1  
            {
				b->color = BLACK;  
				c->parent->color = RED;     
				leftRotate(tree, c->parent);   
				b = c->parent->right;										//新的兄弟节点，这个节点一定是黑色的。这个节点之前是
																			//红色节点的儿子    
			}        

			if (b->right->color == BLACK && b->left->color == BLACK) //case 2 
			{                
				b->color = RED;												//将c的父节点的另一颗子树黑节点减少1      
				c = c->parent;												//将c上移。上移之后，c的黑高度相同了（因为另一颗子树
																			//的根节点由黑变为红）     
			}    
			else    //case 3或case 4     
			{     
				if (b->right->color == BLACK && b->left->color == RED)		//case 3通过以下操作将case 3 转化为case 4   
				{     
					b->color = RED;  
					b->left->color = BLACK;  
					rightRotate(tree, b);                    
					b = c->parent->right;     
				}                //case 4               
				//通过下面的操作，红黑树的性质恢复        
				b->color = b->parent->color;    
				b->parent->color = BLACK;    
				b->right->color = BLACK;    
				leftRotate(tree, c->parent);    
				c = tree->root;												//红黑树性质恢复，结束循环。不用break，是因为while结
																			//束后还要执行c->color = BLACK;            
			}
        }//if (c == c->p->left)       
		else   
		{  
			b = c->parent->left;     
			//b节点是红的，可以说明c和b的父亲节点是黑的。通过以下的操作可以吧case 1转换为case 2,3,4中的一个
			if (b->color == RED) //case 1      
			{      
				b->color = BLACK;     
				c->parent->color = RED;  
				rightRotate(tree, c->parent);     
				b = c->parent->left;										//新的兄弟节点，这个节点一定是黑色的。这个节点之前是
																			//红色节点的儿子        
			}        

			if (b->right->color == BLACK && b->left->color == BLACK) //case 2     
			{               
				b->color = RED;												//将c的父节点的另一颗子树黑节点减少1     
				c = c->parent;												//将c上移。上移之后，c的黑高度相同了（因为另一颗子树
																			//的根节点由黑变为红）      
			} 
			else    //case 3或case 4   
			{      
				if (b->right->color == RED && b->left->color == BLACK)      //case 3通过以下操作将case 3 转化为case 4     
				{     
					b->color = RED;   
					b->right->color = BLACK;    
					leftRotate(tree, b); 
					b = c->parent->left;  
				}                //case 4                
				//通过下面的操作，红黑树的性质恢复         
				b->color = b->parent->color;
                b->parent->color = BLACK;    
				b->left->color = BLACK;    
				rightRotate(tree, c->parent);     
				c = tree->root;												//红黑树性质恢复，结束循环。不用break，是因为while结
																			//束后还要执行c->color = BLACK;            
			}
        }//else   
	} 
	c->color = BLACK;  
	return 0;
}//deleteRB_fixup

/************************************************************************/
//返回最小值，如果t是NULL返回NULL
PRBTNode minimum (PRBTree tree, PRBTNode t)
{
    if (t == tree->nil)
        return NULL;
    while (t->left != tree->nil)
        t = t->left;
    return t;
}//minimum

/************************************************************************/
//给出t的后继的节点。如果没有后继，就返回NULL
PRBTNode next (PRBTree tree, PRBTNode t)
{
    PRBTNode p;																//指示父节点
    if (t->right == tree->nil)
    {
        p = t->parent;
        while (p != tree->nil && p->right == t)
        {
            t = p;
            p = t->parent;
        }
        return p;															//如果是最后一个元素，p的值为NULL
    }
    else
        return minimum(tree, t->right);
}//next

/************************************************************************/
//查找，成功返回找到节点的指针，不成功返回应该插入节点的指针
PRBTNode searchTNode(PRBTree tree, char* input)
{
	PRBTNode t = NULL;    
	   
	int flag = 0;															//用来表示查找进入左边的树（0）还是右边的树（1）  
	
	t = tree->root;                    
	while (t != tree->nil)
	{        
		       
		if (0 > strcmp(input,t->Name))        
		{            
			flag = 0;               
			t = t->left;        
		}        
		else        
		{            
			if ( 0 < strcmp(input,t->Name))            
			{                
				flag = 1;                
				t = t->right;            
			}            
			else            
			{                
				return t;
			}        
		}    
	}//while
	return t;	
}


