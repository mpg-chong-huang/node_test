
void allInit()
{
	int i=0;
	for(i=0; i<62; i++)
		initRB(&tree[i]);                       //红黑树的初始化

	//栈顶节点的初始化
    top = (rec_ID_stack *) malloc(sizeof(rec_ID_stack));
	top->next = NULL;
	
	MemoryPool_init(&Memory);                   //内存池初始化
}

void sockRun(int port,int PlNode)
{
	if(socketInit(port))
	{
		//连接前的准备
		pre4Connect(&WinSocketClient,port);
		Connect2CtrlNode(WinSocketClient,1);
		//心跳线程
		CreateThread( NULL, 0, HR, &port, 0, 0 );
		//服务器解析程序
		socketGetData(PlNode);
	
	}
}