#include"Socket.h"
/*
void ReleaseAllUser()
{
    int IsReleaseFlag = 0;
	unsigned int i = 0;
	if(FALSE == IsConnet)
	{
	   ConnectToSysCtrNode();
	}
		for(i=1;i<=5;i++)
		{

		Message[0] = 'R';                                       //将标识符'R'存放到字符串数组Message的中
		Message[1] = (char) (i+'0');                      //将负荷节点PlNodeID值转换为字符存放到字符串数组Message的中
		Message[2] = '\0';
		    //printf("正在释放用户中，请稍候\n");
			if (SendAndRecvMsg(Message) == TRUE)                  //由函数SendAndRecvMsg的返回值，判断是否依旧和总控节点建立连接
			{
			    if (panduan2 ==CheckMsgFromSysCtrNode())
				{
					if (InputStr[0]=='1')
					{
						IsReleaseFlag++;//printf("成功释放负荷节点PlNode%d上所有用户\n",i);  
					}
					else if (InputStr[0]=='2')
					{
						//printf("负荷节点PlNode%d上用户释放失败\n",i);
					}
                    else if (InputStr[0]=='D'&&InputStr[1]=='S')
					{
						IsReleaseFlag++;//printf("负荷节点PlNode%d没有打开\n",i);
					}
				}

			}        				 			 						 	
		} 
		if (5==IsReleaseFlag)
		{
			printf("所有用户已全部被释放\n");
		}
		else
		{
		  printf("用户没有全部注销成功\n");
		}
}


int  ShutdownSystem()
{
    int IsShutdownFlag = 0;
	//int IsShutdownAllPn = FALSE;
	unsigned int i ;
	if(FALSE == IsConnet)
	{
	   ConnectToSysCtrNode();
	}
	for(i=1;i<=5;i++)
	{

		Message[0] = 'S';                                       //将标识符'R'存放到字符串数组Message的中
		Message[1] = (char) (i+'0');                      //将负荷节点PlNodeID值转换为字符存放到字符串数组Message的中
		Message[2] = '\0';
		
		
		//printf("正在释放用户中，请稍候\n");
		if (SendAndRecvMsg(Message) == TRUE)                  //由函数SendAndRecvMsg的返回值，判断是否依旧和总控节点建立连接
		{
            //if (panduan1 ==CheckMsgFromSysCtrNode())
		//	{
			//	printf("本次测试到此结束，将关闭系统，注销所有用户\n");
			//	Sleep(500);
				
			//	 exit(0);
			//}
			if (panduan2 ==CheckMsgFromSysCtrNode())
			{
				if (InputStr[0]=='1')
				{
					IsShutdownFlag++;//printf("成功释放负荷节点PlNode%d上所有用户\n",i);  
				}
				else if (InputStr[0]=='2')
				{
					//printf("负荷节点PlNode%d上用户释放失败\n",i);
				}
				else //if (InputStr[0]=='D'&&InputStr[1]=='S')
				{
					IsShutdownFlag++;//printf("负荷节点PlNode%d没有打开\n",i);
				}

			}

		} 
		else 
		{
		 return TRUE;
		}
		Sleep(500);
	} 
	if (5==IsShutdownFlag)
	{
		//printf("所有用户已全部被释放\n");
		return TRUE;
	}
	else
	{
	   //printf("用户没有全部注销成功\n");
	   return FALSE;
	}
}

  */