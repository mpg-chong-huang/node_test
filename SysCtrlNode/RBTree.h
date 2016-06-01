#include "MemoryPool.h"

/************************************************************************/
#define RED    0
#define BLACK  1

/************************************************************************/
//������ڵ�
typedef struct RBTNode{ 
	char color;																	//�������ɫ��
    struct RBTNode * parent;													//ָ�򸸽ڵ�ָ��
    struct RBTNode * left;														//ָ������ָ��
    struct RBTNode * right;														//ָ���Һ���ָ��
	
		int ID;																		//������
	char Name[20];
	struct RBTNode   *freeNext;													//�ڴ��free����nextֵ
}RBTNode, * PRBTNode;

/************************************************************************/
//ͳһ�ڵ�
typedef struct RBTree{    
	PRBTNode root;																//����㣬�ýڵ�Ϊ��ɫ
	PRBTNode nil;																//ͳһ�Ŀսڵ㣬�ýڵ��Ǻڵ�
}RBTree, * PRBTree;


/************************************************************************/
//���ں�����ĺ�������
int leftRotate (PRBTree tree, PRBTNode t);										//��������
int rightRotate (PRBTree tree, PRBTNode t);										//��������
PRBTNode insertRB (PRBTree tree, char* input,int ID);							//����
int insertRB_fixup (PRBTree tree, PRBTNode t);									//Ϊ���뱣�ֺ������
PRBTNode deleteRB (PRBTree tree, PRBTNode t);									//ɾ��
int deleteRB_fixup (PRBTree tree, PRBTNode c);									//Ϊɾ�����ֺ������
PRBTNode minimum (PRBTree tree, PRBTNode t);									//�ҵ��Խڵ�tΪ��������������С�ڵ�
PRBTNode next (PRBTree tree, PRBTNode t);										//�ҵ�t�ڵ�ĺ��
PRBTNode searchTNode(PRBTree tree, char *input);								//�����в�����ؼ�����ͬ�Ľڵ�

/************************************************************************/
//���������ı��и�����˳��
int leftRotate (PRBTree tree, PRBTNode t)
{    PRBTNode c;																//����   
	 c = t->right;    

	 if (t->right == tree->nil)													       
		 return 1;                       
	 
	 
	   
	 if (t->parent != tree->nil)												//�ж�t�Ƿ�Ϊ���ڵ�    
	 {        
		 if (t->parent->left == t)												//�ж�x��x�ĸ��ڵ�����ӻ����Һ���            
			 t->parent->left = c;          
		 else            
			 t->parent->right = c;    
	 }    
	 else																		//x�Ǹ��ڵ�
		 tree->root = c;    

	 c->parent = t->parent;														//����y�ĸ��ڵ�   
	 
	 t->right = c->left;                 
	 if (c->left != tree->nil)        
		 c->left->parent = t;    

	 c->left = t;    
	 t->parent = c;    
	 return 0;
}

/************************************************************************/
//�������������ı��и�����˳��
int rightRotate (PRBTree tree, PRBTNode t)
{    
	 PRBTNode c;																//����   
	 c = t->left;    

	 if (t->left == tree->nil)													       
		 return 1;                       
	 
	 
	 //���if-else���ڽ�x�ĸ��׽ڵ��lefx��righx��ָ��y�����x�ĸ��ڵ�Ϊ�����ڣ�������rooxָ��y    
	 if (t->parent != tree->nil)												//�ж�x�Ƿ�Ϊ���ڵ�    
	 {        
		 if (t->parent->left == t)												//�ж�x��x�ĸ��ڵ�����ӻ����Һ���            
			 t->parent->left = c;          
		 else            
			 t->parent->right = c;    
	 }    
	 else																		//c�Ǹ��ڵ�
		 tree->root = c;    

	 c->parent = t->parent;														//����c�ĸ��ڵ�  
	 
	 t->left = c->right;                 
	 if (c->right != tree->nil)        
		 c->right->parent = t;    

	 c->right = t;    
	 t->parent = c;    
	 return 0;
}

/************************************************************************/
//������ĳ�ʼ��
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
//����Ԫ��!!!�ǵò����Ԫ�صĳ�ʼ����pָ��Ϊ��ĸ�ڵ㣬left��right��ֵΪNULL��  
//�ò����������������,����ֵ����NULL���������ʧ�ܣ���ͬ����ֵ����
PRBTNode insertRB (PRBTree tree, char* input,int ID)
{  
	PRBTNode tIn = NULL;													  //ָ��Ҫ����Ľڵ�
	PRBTNode pIn = NULL;                                                      //ָ��Ҫ����ڵ��ַ�ĸ��ڵ�
	int flag = 0;															  //������ʾ��������ߵ�����0�������ұߵ�����1��  
	
	tIn = tree->root;														  //����Ľڵ���root,������Ӧ�ĳ�ʼ��    
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
                
    
	//��tָ�������ڵ�ĵ�ַ������ʼ��      
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
//����Ľڵ�����ƻ�����������ʡ��ú���������Ľڵ��Ƿ��ƻ��˺���������ʡ�����ƻ��ˣ��Ͷ������е�����ʹ����������������
int insertRB_fixup (PRBTree tree, PRBTNode t)
{    
	//ֻ�в���ڵ�ĸ����Ǻ�ɫ�ĲŻ��ƻ�����������ʣ�4.���һ������Ǻ�ģ���ô�����������Ӷ��Ǻڵģ�
	while (t->parent->color == RED)        
	{        if (t->parent->parent->left == t->parent)							//����ڵ�ĸ��ڵ㱾����left        
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
					{//��case 2ת��Ϊ��case 3                            
						t = t->parent;											//�ⲽ��ֵ��Ϊ����ת��Ϊcase 3ʱ��tָ����������
																				//��ڵ㣬��case 3�������һ��                    
						leftRotate(tree, t);                
					} 
					//case 3                
					t->parent->color = BLACK;                
					t->parent->parent->color = RED;                
					rightRotate(tree, t->parent->parent);            
				}        
			}//if        
			else																//����ڵ�ĸ��ڵ㱾����right        
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
					{//��case 2ת��Ϊ��case 3                            
						t = t->parent;											//�ⲽ��ֵ��Ϊ����ת��Ϊcase 3ʱ��tָ���������
																				//�ĺ�ڵ㣬��case 3�������һ��                    
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
//ɾ�����ݡ�Ҫ��������ݽڵ��ָ��
PRBTNode deleteRB (PRBTree tree, PRBTNode t)
{    
	PRBTNode cDele = NULL;														//cָ��Ҫȡ����ɾ���ڵ���ӽڵ�   
	PRBTNode dDele = NULL;						 
	int tmp;				
	char str1[20]={'\0'};

    if (t == tree->nil)					     
	    return NULL;					         
	                                          
		       
//dָ������Ҫɾ����Ԫ�ص��±ꡣ���t��left��right����ֵ����ת��Ϊɾ��t�ĺ�̽ڵ㣬
//���Ѻ�̽ڵ�����ݸ��Ƹ�tָ��Ľڵ㡣
//�����������ֱ��ɾ��tָ��Ľڵ�
	if (t->left != tree->nil && t->right != tree->nil)    
	{      
		dDele = next(tree, t);                     
		tmp = dDele->ID;														//��Ϊʵ�ʲ���Ҫɾ������dָ��Ľڵ㣬�����Ƚ�������
		dDele->ID = t->ID;
        t->ID = tmp;  

		strcpy(str1,dDele->Name);
		strcpy(dDele->Name,t->Name);
		strcpy(t->Name,str1);
	}    
	else   
		dDele = t;                                 
	

	if (dDele->left == tree->nil)   
		cDele = dDele->right;													//ȷ��c��ָ��
	else   
		cDele = dDele->left;                         
    cDele->parent = dDele->parent;												//��c�ĸ���ָ����Ϊd�ĸ���ָ��,c����Ϊ�գ���Ϊ��
																				//��nil�ڵ㣩

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
//ɾ���������������ֺ������
int deleteRB_fixup (PRBTree tree, PRBTNode c)
{ 
	PRBTNode b;																 //�ֵܽڵ�   
	while (c != tree->root && c->color == BLACK)
    {       
		//c�����ӽڵ�
		if (c == c->parent->left)   
		{   
			b = c->parent->right;  
			//b�ڵ��Ǻ�ģ�����˵��c��b�ĸ��׽ڵ��Ǻڵġ�ͨ�����µĲ������԰�case 1ת��Ϊcase 2,3,4�е�һ�� 
			if (b->color == RED) //case 1  
            {
				b->color = BLACK;  
				c->parent->color = RED;     
				leftRotate(tree, c->parent);   
				b = c->parent->right;										//�µ��ֵܽڵ㣬����ڵ�һ���Ǻ�ɫ�ġ�����ڵ�֮ǰ��
																			//��ɫ�ڵ�Ķ���    
			}        

			if (b->right->color == BLACK && b->left->color == BLACK) //case 2 
			{                
				b->color = RED;												//��c�ĸ��ڵ����һ�������ڽڵ����1      
				c = c->parent;												//��c���ơ�����֮��c�ĺڸ߶���ͬ�ˣ���Ϊ��һ������
																			//�ĸ��ڵ��ɺڱ�Ϊ�죩     
			}    
			else    //case 3��case 4     
			{     
				if (b->right->color == BLACK && b->left->color == RED)		//case 3ͨ�����²�����case 3 ת��Ϊcase 4   
				{     
					b->color = RED;  
					b->left->color = BLACK;  
					rightRotate(tree, b);                    
					b = c->parent->right;     
				}                //case 4               
				//ͨ������Ĳ���������������ʻָ�        
				b->color = b->parent->color;    
				b->parent->color = BLACK;    
				b->right->color = BLACK;    
				leftRotate(tree, c->parent);    
				c = tree->root;												//��������ʻָ�������ѭ��������break������Ϊwhile��
																			//����Ҫִ��c->color = BLACK;            
			}
        }//if (c == c->p->left)       
		else   
		{  
			b = c->parent->left;     
			//b�ڵ��Ǻ�ģ�����˵��c��b�ĸ��׽ڵ��Ǻڵġ�ͨ�����µĲ������԰�case 1ת��Ϊcase 2,3,4�е�һ��
			if (b->color == RED) //case 1      
			{      
				b->color = BLACK;     
				c->parent->color = RED;  
				rightRotate(tree, c->parent);     
				b = c->parent->left;										//�µ��ֵܽڵ㣬����ڵ�һ���Ǻ�ɫ�ġ�����ڵ�֮ǰ��
																			//��ɫ�ڵ�Ķ���        
			}        

			if (b->right->color == BLACK && b->left->color == BLACK) //case 2     
			{               
				b->color = RED;												//��c�ĸ��ڵ����һ�������ڽڵ����1     
				c = c->parent;												//��c���ơ�����֮��c�ĺڸ߶���ͬ�ˣ���Ϊ��һ������
																			//�ĸ��ڵ��ɺڱ�Ϊ�죩      
			} 
			else    //case 3��case 4   
			{      
				if (b->right->color == RED && b->left->color == BLACK)      //case 3ͨ�����²�����case 3 ת��Ϊcase 4     
				{     
					b->color = RED;   
					b->right->color = BLACK;    
					leftRotate(tree, b); 
					b = c->parent->left;  
				}                //case 4                
				//ͨ������Ĳ���������������ʻָ�         
				b->color = b->parent->color;
                b->parent->color = BLACK;    
				b->left->color = BLACK;    
				rightRotate(tree, c->parent);     
				c = tree->root;												//��������ʻָ�������ѭ��������break������Ϊwhile��
																			//����Ҫִ��c->color = BLACK;            
			}
        }//else   
	} 
	c->color = BLACK;  
	return 0;
}//deleteRB_fixup

/************************************************************************/
//������Сֵ�����t��NULL����NULL
PRBTNode minimum (PRBTree tree, PRBTNode t)
{
    if (t == tree->nil)
        return NULL;
    while (t->left != tree->nil)
        t = t->left;
    return t;
}//minimum

/************************************************************************/
//����t�ĺ�̵Ľڵ㡣���û�к�̣��ͷ���NULL
PRBTNode next (PRBTree tree, PRBTNode t)
{
    PRBTNode p;																//ָʾ���ڵ�
    if (t->right == tree->nil)
    {
        p = t->parent;
        while (p != tree->nil && p->right == t)
        {
            t = p;
            p = t->parent;
        }
        return p;															//��������һ��Ԫ�أ�p��ֵΪNULL
    }
    else
        return minimum(tree, t->right);
}//next

/************************************************************************/
//���ң��ɹ������ҵ��ڵ��ָ�룬���ɹ�����Ӧ�ò���ڵ��ָ��
PRBTNode searchTNode(PRBTree tree, char* input)
{
	PRBTNode t = NULL;    
	   
	int flag = 0;															//������ʾ���ҽ�����ߵ�����0�������ұߵ�����1��  
	
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


