#include "HelpMsg.h"
#include "ProdRandUserName.h"
#include "CommWithSysCtrN.h"
#include "CommWithSysCtrN2.h"
void ConReq_GetPn_RelReq(int n,char * nStr,int * a)       //�������ܣ������½�����ѯ���ͷ�n���û�
{
	int i;
	int PlnodeID;
	n = atoi(nStr); 
	ProduceRandomUserNameArray(a);                        //����������ظ��û������顣
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 10);        //����������ɫΪ����
	for (i=1;i<=n;i++)
	{
		itoa(a[i],nStr,10);
		ConnectReq(nStr);
		Sleep(20);
	}
	//Sleep(1000);
	printf("\n�½��û�%d�β������\n",n);	
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 14);        //����������ɫΪ����
	for (i=1;i<=n;i++)
	{
		itoa(a[i],nStr,10);  
		PlnodeID = GetPnodeByUser(nStr);
		if (PlnodeID!=-1)
		{
			printf("����ѯ���û�%s�ڸ��ɽڵ�Plnode%d��\n",nStr,PlnodeID); //����û����ڸ��ɽڵ�PlNode	
		}                  							
		Sleep(20);
	}
	printf("\n��ѯ�û�%d�β������\n",n);	
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 13);        //����������ɫΪ����				
	for (i=1;i<=n;i++)
	{
		itoa(a[i],nStr,10);
		ReleaseReq(nStr);
		Sleep(20);
	}
	//Sleep(1000);
	printf("\n�ͷ��û�%d�β������\n",n);	
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);                  //����������ɫΪ����
    printf("\n�½�����ѯ���ͷ��û���%d�β������\n",n);
}

void go()
{
	char nStr[100];
	char Input[100];                                                             //�������������ַ���
    char InputBackup [100];   
	char   UserName[20];                                                         //����û���
	int     PlnodeId;                                                            //��ʾ���ɽڵ�PlNodeID,���ɽڵ�PlNodeID��ʶ���ɽڵ�PlNode
	char    PlnodeIdStr[20] ;                                                    //��Ÿ��ɽڵ�PlNodeID���ַ�������
	unsigned int UserNumOnPlnode;                                                //��ʾ�ڵ���̵�ǰ������û���
	unsigned int i;                                                              //iΪ����forѭ���о���ʹ�õ�����������
    unsigned int UserNameInt = 1000;	
	unsigned int length;
	int n;
	int a [1001];
	while(1)                                                                         
	{
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15); 		
		printf("\n����������>");
		gets(Input);	                                                          //���������ַ���		
		if (strcmp(Input,"")==0)
		{
			printf("���벻��Ϊ��,����������\n");
		}
		else
		{
			strcpy(InputBackup,Input);
			if (!(Input[0]=='+'||Input[0]=='~'||Input[0]=='#'||Input[0]=='@'))
			{
				strupr(Input);
			}

			switch(Input[0])                                                       //���ݱ�ʶ��'+'��'~'��'#'��'@'ѡ��ͬ����
			{   
				case '+' :                                                         //ʵ���½��û�����
				{						
					for (i=1; i<=strlen(Input); i++)                               //ȥ����ʶ��'+',�������ַ����е��û�����ŵ�UserName������
					{
						UserName[i-1]=Input[i];
					}
					if(strcmp(UserName,"LOGOUT")==0)
					{
					   printf("'%s'�������ʶ�������������û���������������\n",UserName);
					}
					else
					{
					    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 10);   //����������ɫΪ����
					    ConnectReq (UserName);                                          //��SysCtrlNode���̷���Ϣ֪ͨ�û�����
					}

				}; break;
				case '~' :                                                             //ʵ���ͷ��û�����                   
				{
					for (i=1; i<=strlen(Input); i++)                                   //ȥ����ʶ��'~',�������ַ����е��û�����ŵ�UserName������
					{
						UserName[i-1]=Input[i];
					}
						SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 13);        //����������ɫΪ����
						ReleaseReq (UserName);                                               //��SysCtrlNode���̷���Ϣ֪ͨ�û��ͷ�

				};break;
				case '#' :                                                               //ʵ�ֲ�ѯ�û�����
				{
					for (i=1; i<=strlen(Input); i++)                                    //ȥ����ʶ��'#',�������ַ����е��û�����ŵ�UserName������                                
					{
						UserName[i-1]=Input[i];
					}
					if(strcmp(UserName,"LOGOUT")==0)
					{
					   printf("'%s'�������ʶ�������������û���������������\n",UserName);
					}
					else
					{
						SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 14);         //����������ɫΪ����
						PlnodeId = GetPnodeByUser(UserName);	                              //�����û��ĸ��ɽڵ�PlNodePlNode��ʶ����ЧֵΪ-1
						if (PlnodeId==-1)              
						{
							//printf("�û������Ϸ�,���ߴ��û�����5�����ɽڵ�PlNode������,�����ܿؽڵ����������æ�С�����\n");              //��ʾ˵���û�����3�����ɽڵ�PlNode��
						}
				
						else                             
						{
							printf("����ѯ���û��ڸ��ɽڵ�Plnode%d��\n",PlnodeId);	        //����û����ڸ��ɽڵ�PlNode				
						}
					}

				};break;			
				case '@' :                                                                //ʵ�ֲ�ѯ���ɽڵ�PlNode���̵�ǰ������û����Ĺ���
				{                                                                     //ʵ����һ��������Ҫ��2���ж����������ַ����Ƿ�Ϸ�
					if( !( strcmp(Input,"@1")==0||strcmp(Input,"@2")==0||strcmp(Input,"@3")==0 ||strcmp(Input,"@4")==0||strcmp(Input,"@5")==0 ))
					{
						SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 11);                //����������ɫΪ����			
						printf("'%s'���ǺϷ�����,���������븺�ɽڵ��ʶ\n",InputBackup);
					}
					else
					{
						for (i=1; i<=strlen(Input); i++)                                 //ȥ����ʶ��'@',�������ַ����и��ɽڵ�PlNodeID�Ĵ�ŵ�PlnodeIdStr������
						{			
							PlnodeIdStr[i-1]=Input[i]; 
						}
						PlnodeId = atoi(PlnodeIdStr);                                        //�����ɽڵ�PlNodeID���ַ���ת��Ϊ�޷�������
						SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 11);      //����������ɫΪ����
						UserNumOnPlnode =  GetUserNumByPnode(PlnodeId);                      //��ø��ɽڵ�PlNode���̵�ǰ������û���
						if (UserNumOnPlnode>0)
						{
							printf("���ɽڵ�Plnode%d��ǰ������û���Ϊ%d\n",PlnodeId,UserNumOnPlnode);	  //������ɽڵ�PlNode���̵�ǰ������û���														
						}						   
					}			             
				};break;
				case 'H':
				{
					if(strcmp(Input,"HELP")==0)
					{						
					   printHelpMessage();										
					}
					else
					{
						printf("'%s'���ǺϷ�����,����������\n",InputBackup);
					}				
				}break;
				case 'C':
				{
					if (strlen(Input)==3&&Input[0]=='C'&&Input[1]=='G'&&Input[2]=='R')
					{
						printf("������nֵ��n��ȡֵ��Χ��0<n<=1000��>");
						gets(nStr);                                                           //����nֵ
						length = strlen(nStr);
						i = 0;	
						while( i<length || length == 0)
						{
							n = atoi(nStr); 
							if (  nStr[i]<'0' || nStr[i]>'9' || length>4 || n<=0 || n>1000 || nStr[i] == ' ')
							{
								printf("����nֵ���Ϸ�������������n��");									
								gets(nStr);                   
								length = strlen(nStr);
								i = 0;		
							}
							else
							{
								i++;
							}								    																
						}
						ConReq_GetPn_RelReq(n,nStr,a);		
					}
					else
					{
						printf("'%s'���ǺϷ�����,����������\n",InputBackup);
					}
				}break;
				case 'L':
				{
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 9);        
					if(strcmp(Input,"LOGOUT")==0)
					{
						ReleaseAllUser(); 
					}
					else
					{
						SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);                  //����������ɫΪ����		  
					  printf("'%s'���ǺϷ�����,����������\n",InputBackup);
					}
				}break;
             case 'E':
					{
						if(strcmp(Input,"EXIT")==0)
						{
							printf("�Ƿ�ע�������û���������Y��N��");
							gets(Input);
							do
							{
								if (strcmp(Input,"Y")==0||strcmp(Input,"y")==0)
								{
									SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 9); 
									printf("����ע�������û�������\n");
									ReleaseAllUser();									
								}
								else if (strcmp(Input,"N")==0||strcmp(Input,"n")==0)
								{
									printf("���򲻻�ע���û�\n");									
								}
								else
								{
									 printf("���벻�Ϸ�������������Y��N��");
									 gets(Input);
								}						
							}
							while (!(strcmp(Input,"Y")==0||strcmp(Input,"y")==0||strcmp(Input,"N")==0||strcmp(Input,"n")==0));							
						    printf("������3���ر�\n");
						    Sleep(3000);							
							exit(0);
						}
						else
						{
							SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);                  //����������ɫΪ����		  
						  printf("'%s'���ǺϷ�����,����������\n",InputBackup);
						}				
					}break;
				case 'S':
				{
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 9);        
					if(strcmp(Input,"SHUTDOWN")==0)
					{
						ShutdownSystem();
					}
					else
					{
						SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);                  //����������ɫΪ����		  
					  printf("'%s'���ǺϷ�����,����������\n",InputBackup);
					}
				}break;
				default: 
				{
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);                  //����������ɫΪ����	  
					printf("'%s'���ǺϷ�����,����������\n",InputBackup);				
				}break;										
		}	//end of switch
	}
 }//end of while(1)	 
}

int main()
{
	
    printPrefaceMessage();
	go();
	
return 0;
}//end of main



					
				
					