#include "Socket.h"
int UserNameCheck (char * Input)                                //函数实现功能：判断用户名是否合法
{
	unsigned int i=0;
	if( (strlen(Input)==0)|| ( strlen (Input)>16 || strlen(Input)<4 ) ||Input[0]=='_' )
	{                                                             
		return	FALSE;                                          //如果用户名不在4-16个字符之内或用户名第一个字符是下划线，返回FALSE
	}
	while( i< strlen(Input) )
	{
		if (  Input[i]=='_' || (Input[i]>='0'&&Input[i]<='9') || (Input[i]>='a'&&Input[i]<='z') || (Input[i]>='A'&&Input[i]<='Z') )
		{
			i++;
		}
		else
		{
			return 	FALSE;	                                     //如果用户名的字符取值不都是英文字母、数字或下划线，返回FALSE
		}
	}
	return TRUE;                                                 //如果用户名的字符取值是英文字母、数字或下划线，返回TRUE	
}//end of UserNameCheck (char * Input) 

void ConnectReq(char * UserName)                                 //函数实现功能：向SysCtrlNode进程发消息通知用户UserName建立
{	       
    unsigned int i;
	if (UserNameCheck(UserName)==FALSE)                           
	{                                                            //如果UserNameCheck函数的返回值为FALSE，提示建立用户失败
		printf("用户名'%s'不合法,建立用户失败。\n",UserName);                                      
	}
	else                                                         //如果UserNameCheck函数的返回值为TRUE，执行下面代码
	{
			if(FALSE == IsConnet)
			{
			   ConnectToSysCtrNode();
			}    				 
				Message[0] = '+';                                 //将标识符'+'存放到字符串数组Message的中
				for (i=0; i<=strlen(UserName); i++)               //要用户名存放到字符串数组Message中
					Message[i+1]=UserName[i];
					if (SendAndRecvMsg(Message) == TRUE)           //由函数SendAndRecvMsg的返回值，判断是否依旧和总控节点建立连接
					{							
							if(panduan2 == CheckMsgFromSysCtrNode())
							{
                                    if (InputStr[0]=='2')                                        
									{
										printf("用户%s已接入服务\n",UserName);    //InputStr[0]值为2,表示新建用户与其他用户重名
									}
									else if (InputStr[0]=='1')                    
									{
										printf("用户%s建立成功,已接入服务\n",UserName);    //InputStr[0]值为1,表示用户建立成功
									}
							}
					}   				 			 						 	
			
	}     //end of if (UserNameCheck(UserName)==FALSE)
}        
//end of ConnectReq(char * UserName) 

void ReleaseReq(char * UserName)                                //函数实现功能：向SysCtrlNode进程发消息通知用户A释放
{
	unsigned int i;  
	if (UserNameCheck(UserName)==FALSE)
	{
		printf("用户名'%s'不合法，不存在要释放的用户%s\n",UserName,UserName);        //如果UserNameCheck函数的返回值为FALSE，提示要释放的用户不合法
	}
	else                                                        //如果UserNameCheck函数的返回值为TRUE，执行下面代码
	{
			if(FALSE == IsConnet)
			{
			   ConnectToSysCtrNode();
			}		
			Message[0] = '~';                                   //将标识符'~'存放到字符串数组Message的中			
			for (i=0; i<=strlen(UserName); i++)                 //要用户名存放到字符串数组Message中
						Message[i+1]=UserName[i];
			if ( TRUE == SendAndRecvMsg(Message))
			{
					if(panduan2 == CheckMsgFromSysCtrNode())				
					{
                            if (InputStr[0]=='3')                                    
							{
								printf("要释放的用户%s不存在\n",UserName);  //如果InputStr[0]值为3,表示要释放的用户不存在
							}
							else if (InputStr[0]=='1')                              
							{
								printf("成功释放用户%s\n",UserName);        //如果InputStr[0]值为1,表示成功释放用户
							}
					}
			}    //end of if(IsConnet2 == TRUE)
	}    //end of if (UserNameCheck(UserName)==FALSE)		

}  

int GetPnodeByUser(char * UserName)                                //函数实现功能：返回用户UserName的Pnode标识,即PnodeId。
{
	unsigned int i=0;       
	if (UserNameCheck(UserName)==FALSE)
	{
		printf("用户名'%s'不合法，不存在要查询的用户%s\n",UserName,UserName); //如果UserNameCheck函数的返回值为FALSE，提示要查询的用户不合法
		return -1;                                          //返回无效值-1
	}
    //UserNameCheck函数的返回值为TRUE，则执行下面代码
	if(FALSE == IsConnet)
	{
	   ConnectToSysCtrNode();
	}
	Message[0] = '#';                                               //将标识符'#'存放到字符串数组Message的中
	for (i=0; i<=strlen(UserName); i++)                             //要用户名存放到字符串数组Message中
	{	  
		Message[i+1]=UserName[i];
	}
	if ( TRUE == SendAndRecvMsg(Message))
	{
			if(panduan1 == CheckMsgFromSysCtrNode())
			{
				return -1;
			}
            //函数CheckMsgFromSysCtrNode()的返回值为panduan2，则执行下面代码
  			if (InputStr[0]=='0')               
			{				                                        //如果InputStr[0]值为0,表示用户不在5个Pnode节点中
				printf("要查询的用户%s不存在\n",UserName);
				return -1;                                          //返回无效值-1
			}
			else                             
			{                                                       //这里InputStr[0]值表示用户所在Pnode节点                                   				
				return atoi(InputStr);                                  //返回用户所在Pnode节点
			}
	}				
    return -1;
}

unsigned int GetUserNumByPnode(int PlnodeID)                        //返回负荷节点PlNode进程当前处理的用户数
{         			
	if (PlnodeID>5||PlnodeID<=0)                                    //判断负荷节点PlNodeID值是否在1-5内
	{
		printf("输入负荷节点的ID'%d'不合法，不存在负荷节点Plnode%d\n",PlnodeID,PlnodeID);
		return	0;                                                  //如果负荷节点PlNodeID值不合法，返回0
	}
     //负荷节点PlNodeID值有效，执行下面代码
	if(FALSE == IsConnet)
	{
	   ConnectToSysCtrNode();
	}
	Message[0] = '@';                                               //将标识符'#'存放到字符串数组Message的中
	Message[1] = (char) (PlnodeID+'0');                             //将负荷节点PlNodeID值转换为字符存放到字符串数组Message的中
	Message[2] = '\0';
	if (TRUE == SendAndRecvMsg(Message))
	{
		if(panduan2 ==CheckMsgFromSysCtrNode())
		{
			if (InputStr[0]=='0')               
			{				                                        //如果InputStr[0]值为0,表示用户不在5个Pnode节点中
				printf("负荷节点Plnode%d当前处理的用户数为0\n",PlnodeID);
				return 0;                                           //返回无效值0
			}
			if (InputStr[0]=='D'&&InputStr[1]=='S')                 //如果InputStr[0]=='D'&&InputStr[1]=='S',表示负荷节点没有打开
			{
				printf("负荷节点Plnode%d没有打开，Plnode%d没有用户接入\n",PlnodeID,PlnodeID);
			}
			else                             
			{                                                       //这里atoi(InputStr)值表示用户所在Pnode节点                                   				
				return atoi(InputStr);                              //返回用户总数
			}
		}			
	}       
    return 0;
}
