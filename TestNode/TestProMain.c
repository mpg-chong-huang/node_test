#include "HelpMsg.h"
#include "ProdRandUserName.h"
#include "CommWithSysCtrN.h"
#include "CommWithSysCtrN2.h"
void ConReq_GetPn_RelReq(int n,char * nStr,int * a)       //函数功能：连续新建、查询、释放n个用户
{
	int i;
	int PlnodeID;
	n = atoi(nStr); 
	ProduceRandomUserNameArray(a);                        //产生随机不重复用户名数组。
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 10);        //设置字体颜色为亮绿
	for (i=1;i<=n;i++)
	{
		itoa(a[i],nStr,10);
		ConnectReq(nStr);
		Sleep(20);
	}
	//Sleep(1000);
	printf("\n新建用户%d次操作完成\n",n);	
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 14);        //设置字体颜色为亮黄
	for (i=1;i<=n;i++)
	{
		itoa(a[i],nStr,10);  
		PlnodeID = GetPnodeByUser(nStr);
		if (PlnodeID!=-1)
		{
			printf("经查询，用户%s在负荷节点Plnode%d中\n",nStr,PlnodeID); //输出用户所在负荷节点PlNode	
		}                  							
		Sleep(20);
	}
	printf("\n查询用户%d次操作完成\n",n);	
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 13);        //设置字体颜色为亮紫				
	for (i=1;i<=n;i++)
	{
		itoa(a[i],nStr,10);
		ReleaseReq(nStr);
		Sleep(20);
	}
	//Sleep(1000);
	printf("\n释放用户%d次操作完成\n",n);	
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);                  //设置字体颜色为亮白
    printf("\n新建、查询和释放用户各%d次操作完成\n",n);
}

void go()
{
	char nStr[100];
	char Input[100];                                                             //存放输入命令的字符串
    char InputBackup [100];   
	char   UserName[20];                                                         //存放用户名
	int     PlnodeId;                                                            //表示负荷节点PlNodeID,负荷节点PlNodeID标识负荷节点PlNode
	char    PlnodeIdStr[20] ;                                                    //存放负荷节点PlNodeID的字符串类型
	unsigned int UserNumOnPlnode;                                                //表示节点进程当前处理的用户数
	unsigned int i;                                                              //i为代码for循环中经常使用到的自增变量
    unsigned int UserNameInt = 1000;	
	unsigned int length;
	int n;
	int a [1001];
	while(1)                                                                         
	{
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15); 		
		printf("\n请输入命令>");
		gets(Input);	                                                          //输入命令字符串		
		if (strcmp(Input,"")==0)
		{
			printf("输入不能为空,请重新输入\n");
		}
		else
		{
			strcpy(InputBackup,Input);
			if (!(Input[0]=='+'||Input[0]=='~'||Input[0]=='#'||Input[0]=='@'))
			{
				strupr(Input);
			}

			switch(Input[0])                                                       //根据标识符'+'、'~'、'#'、'@'选择不同功能
			{   
				case '+' :                                                         //实现新建用户功能
				{						
					for (i=1; i<=strlen(Input); i++)                               //去除标识符'+',将命令字符串中的用户名存放到UserName数组中
					{
						UserName[i-1]=Input[i];
					}
					if(strcmp(UserName,"LOGOUT")==0)
					{
					   printf("'%s'是命令标识符，不能用作用户名，请重新输入\n",UserName);
					}
					else
					{
					    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 10);   //设置字体颜色为亮绿
					    ConnectReq (UserName);                                          //向SysCtrlNode进程发消息通知用户建立
					}

				}; break;
				case '~' :                                                             //实现释放用户功能                   
				{
					for (i=1; i<=strlen(Input); i++)                                   //去除标识符'~',将命令字符串中的用户名存放到UserName数组中
					{
						UserName[i-1]=Input[i];
					}
						SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 13);        //设置字体颜色为亮紫
						ReleaseReq (UserName);                                               //向SysCtrlNode进程发消息通知用户释放

				};break;
				case '#' :                                                               //实现查询用户功能
				{
					for (i=1; i<=strlen(Input); i++)                                    //去除标识符'#',将命令字符串中的用户名存放到UserName数组中                                
					{
						UserName[i-1]=Input[i];
					}
					if(strcmp(UserName,"LOGOUT")==0)
					{
					   printf("'%s'是命令标识符，不能用作用户名，请重新输入\n",UserName);
					}
					else
					{
						SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 14);         //设置字体颜色为亮黄
						PlnodeId = GetPnodeByUser(UserName);	                              //返回用户的负荷节点PlNodePlNode标识。无效值为-1
						if (PlnodeId==-1)              
						{
							//printf("用户名不合法,或者此用户不在5个负荷节点PlNode进程中,或者总控节点服务器正繁忙中。。。\n");              //提示说明用户不在3个负荷节点PlNode中
						}
				
						else                             
						{
							printf("经查询，用户在负荷节点Plnode%d中\n",PlnodeId);	        //输出用户所在负荷节点PlNode				
						}
					}

				};break;			
				case '@' :                                                                //实现查询负荷节点PlNode进程当前处理的用户数的功能
				{                                                                     //实现这一功能这里要第2次判断输入命令字符串是否合法
					if( !( strcmp(Input,"@1")==0||strcmp(Input,"@2")==0||strcmp(Input,"@3")==0 ||strcmp(Input,"@4")==0||strcmp(Input,"@5")==0 ))
					{
						SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 11);                //设置字体颜色为亮白			
						printf("'%s'不是合法命令,请重新输入负荷节点标识\n",InputBackup);
					}
					else
					{
						for (i=1; i<=strlen(Input); i++)                                 //去除标识符'@',将命令字符串中负荷节点PlNodeID的存放到PlnodeIdStr数组中
						{			
							PlnodeIdStr[i-1]=Input[i]; 
						}
						PlnodeId = atoi(PlnodeIdStr);                                        //将负荷节点PlNodeID的字符串转换为无符号整型
						SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 11);      //设置字体颜色为亮青
						UserNumOnPlnode =  GetUserNumByPnode(PlnodeId);                      //获得负荷节点PlNode进程当前处理的用户数
						if (UserNumOnPlnode>0)
						{
							printf("负荷节点Plnode%d当前处理的用户数为%d\n",PlnodeId,UserNumOnPlnode);	  //输出负荷节点PlNode进程当前处理的用户数														
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
						printf("'%s'不是合法命令,请重新输入\n",InputBackup);
					}				
				}break;
				case 'C':
				{
					if (strlen(Input)==3&&Input[0]=='C'&&Input[1]=='G'&&Input[2]=='R')
					{
						printf("请输入n值，n的取值范围：0<n<=1000。>");
						gets(nStr);                                                           //输入n值
						length = strlen(nStr);
						i = 0;	
						while( i<length || length == 0)
						{
							n = atoi(nStr); 
							if (  nStr[i]<'0' || nStr[i]>'9' || length>4 || n<=0 || n>1000 || nStr[i] == ' ')
							{
								printf("输入n值不合法，请重新输入n：");									
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
						printf("'%s'不是合法命令,请重新输入\n",InputBackup);
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
						SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);                  //设置字体颜色为亮白		  
					  printf("'%s'不是合法命令,请重新输入\n",InputBackup);
					}
				}break;
             case 'E':
					{
						if(strcmp(Input,"EXIT")==0)
						{
							printf("是否注销所有用户？请输入Y或N：");
							gets(Input);
							do
							{
								if (strcmp(Input,"Y")==0||strcmp(Input,"y")==0)
								{
									SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 9); 
									printf("正在注销所有用户。。。\n");
									ReleaseAllUser();									
								}
								else if (strcmp(Input,"N")==0||strcmp(Input,"n")==0)
								{
									printf("程序不会注销用户\n");									
								}
								else
								{
									 printf("输入不合法，请重新输入Y或N：");
									 gets(Input);
								}						
							}
							while (!(strcmp(Input,"Y")==0||strcmp(Input,"y")==0||strcmp(Input,"N")==0||strcmp(Input,"n")==0));							
						    printf("程序将在3秒后关闭\n");
						    Sleep(3000);							
							exit(0);
						}
						else
						{
							SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);                  //设置字体颜色为亮白		  
						  printf("'%s'不是合法命令,请重新输入\n",InputBackup);
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
						SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);                  //设置字体颜色为亮白		  
					  printf("'%s'不是合法命令,请重新输入\n",InputBackup);
					}
				}break;
				default: 
				{
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);                  //设置字体颜色为亮白	  
					printf("'%s'不是合法命令,请重新输入\n",InputBackup);				
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



					
				
					