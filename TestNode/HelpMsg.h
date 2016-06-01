#include "Socket.h"
#ifndef _HELPMSG_H
#define _HELPMSG_H
void printPrefaceMessage()
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);                  //设置字体颜色为亮白
	printf("\t\t\t\t《测试进程》\n使用者请按要求输入合法命令\n");	
	printf("主要功能命令如下\n");
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 10); 
	printf("1、新增用户A, 输入命令：+A \n");
	printf("2、释放用户A, 输入命令：~A \n");
	printf("3、查询接入用户A的负荷节点PlNode的ID, 输入命令：#A \n");
	printf("4、查询负荷节点PlNodei上用户个数, 输入命令：@i \n");	
    printf("5、连续新建、查询、释放n个用户，输入命令：CGR ，回车后在输入数值n  \n");
	printf("6、注销所有用户，输入命令：LOGOUT\n");
	printf("7、关闭测试进程，输入命令：EXIT\n");
	printf("8、退出仿真系统，输入命令：SHUTDOWN\n");
	printf("9、查看命令详细使用说明，输入命令:HELP\n");
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);                  //设置字体颜色为亮白
	printf("A表示用户名,其取值必须在4-16个字符之内,字符只能是英文字符、数字和下划线,第1个字符不能为下划线.\n");		
	printf("i表示负荷节点PlNode标识,其取值只能是1、2、3、4、5\n");
	printf("n表示1个数值，其取值为：0<n<=1000\n");
	printf("命令支持英文字符大小写输入。如输入CGR与输入cgr意思一样\n");
	printf("程序每执行完一次命令后，将等待使用者输入新命令\n");
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 14); 
	printf("刚开始进行测试时，请开启SysCtrPlnode.exe程序，否则测试没意义\n");	
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 11); 
	printf("结束测试时，请在关闭测试进程前使用SHUTDOWN或LOGOUT命令，\n");
    printf("或者手动删除PlNode.BU文件，以保证下一次测试环境是纯净的\n");
    
}

void printHelpMessage()
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 10);
	printf("===================== 输入命令说明信息 ======================\n");
	printf("命令           功能说明和使用说明\n");
	printf("HELP           提供输入命令的说明信息\n\n");
	printf("+              新增用户,如新建用户A，输入：+A\n");
	printf("               A表示用户名,其取值必须在4-16个字符之内,字符只能是英文字母、\n");
	printf("               数字和下划线,第1个字符不能为下划线\n\n");
	printf("~              释放用户A,输入如：~A\n");
	printf("               A表示用户名,其取值必须在4-16个字符之内,\n");
	printf("               字符只能是英文字母、数字和下划线,第1个字符不能为下划线\n\n");
	printf("#              查询用户A所在负荷节点PlNode的ID,输入如：#A\n");
	printf("               A表示用户名,其取值必须在4-16个字符之内,\n");
	printf("               字符只能是英文字符、数字和下划线,第1个字符不能为下划线\n\n");
	printf("@              查询负荷节点PlNodei上用户个数,输入如：@i\n");
	printf("               i表示负荷节点PlNode标识,其取值只能是1、2、3、4、5\n\n");																	
	printf("CGR            依次新建、查询和释放n个用户,如新建、查询和释放10用户,\n");
	printf("               输入：CGR ,回车后在输入:10\n");
	printf("               n的取值为：0<n<=1000。用户名从1000-100000中随机生成\n");
	printf("LOGOUT         注销所有用户服务,被注销的用户服务不会恢复\n\n");
	printf("EXIT           关闭测试进程\n\n");
    printf("SHUTDOWN       关闭仿真系统和测试进程\n\n");
	printf("======================= 说明信息结束 ========================\n");		
}

#endif