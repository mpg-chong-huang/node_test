
void allInit()
{
	int i=0;
	for(i=0; i<62; i++)
		initRB(&tree[i]);                       //������ĳ�ʼ��

	//ջ���ڵ�ĳ�ʼ��
    top = (rec_ID_stack *) malloc(sizeof(rec_ID_stack));
	top->next = NULL;
	
	MemoryPool_init(&Memory);                   //�ڴ�س�ʼ��
}

void sockRun(int port,int PlNode)
{
	if(socketInit(port))
	{
		//����ǰ��׼��
		pre4Connect(&WinSocketClient,port);
		Connect2CtrlNode(WinSocketClient,1);
		//�����߳�
		CreateThread( NULL, 0, HR, &port, 0, 0 );
		//��������������
		socketGetData(PlNode);
	
	}
}