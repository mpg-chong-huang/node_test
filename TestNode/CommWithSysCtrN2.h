#include"Socket.h"
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
			Message[1] = (char) (i+'0');                            //将负荷节点PlNodeID值转换为字符存放到字符串数组Message的中
			Message[2] = '\0';
			if (SendAndRecvMsg(Message) == TRUE)                     //由函数SendAndRecvMsg的返回值，判断是否依旧和总控节点建立连接
			{
				if (panduan2 ==CheckMsgFromSysCtrNode())
				{
					if (InputStr[0]=='1')
					{
						IsReleaseFlag++;  
					}
					else if (InputStr[0]=='2')
					{
						//printf("负荷节点PlNode%d上用户释放失败\n",i);
					}
					else if (InputStr[0]=='D'&&InputStr[1]=='S')
					{
						IsReleaseFlag++;
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

void  ShutdownSystem()
{
    int IsShutdownFlag = 0;
	unsigned int i ;
	printf("将依次自动关闭负荷节点程序、总控节点程序和测试进程程序\n");
	Sleep(500);	
	if(FALSE == IsConnet)
	{
	   ConnectToSysCtrNode();
	}
	for(i=1;i<=5;i++)
	{
		Message[0] = 'S';                                       //将标识符'R'存放到字符串数组Message的中
		Message[1] = (char) (i+'0');                           //将负荷节点PlNodeID值转换为字符存放到字符串数组Message的中
		Message[2] = '\0';
		if (SendAndRecvMsg(Message) == TRUE)                  //由函数SendAndRecvMsg的返回值，判断是否依旧和总控节点建立连接
		{
			if (panduan2 ==CheckMsgFromSysCtrNode())
			{
				if (InputStr[0]=='1')
				{
					IsShutdownFlag++;  
				}
				else if (InputStr[0]=='2')
				{
					//printf("负荷节点PlNode%d上用户释放失败\n",i);
				}
				else 
				{
					IsShutdownFlag++;
				}
			}
		} 
		Sleep(500);
	} 
	if (5==IsShutdownFlag)
	{
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);                  //设置字体颜色为亮白
		printf("测试进程将在3秒后关闭\n");
		Sleep(3000);							
		exit(0);
	}
	else
	{
	   printf("关闭系统失败\n");
	}
}



