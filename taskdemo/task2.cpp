#include"task2.h"

CTask2::CTask2()
{
	m_pTask = NULL;
}

CTask2::~CTask2()
{
}

bool CTask2::Init(void* pvParam)
{
	bool	bRet = false;

	if (NULL == pvParam)
	{
		printf("invalid param.");
		goto END;
	}

	m_pTask = (PTASK_2)pvParam;

	bRet = true;
END:
	return bRet;
}

bool CTask2::Excute()
{
	bool bRet = false;
	TASK_2_RET taskRet = { 0 };

	printf("task 2 excute, data is %s.\n", m_pTask->data.szMsg);

	taskRet.header.nType = m_pTask->header.nType;
	taskRet.ret.nRet = 2;

	bRet = m_pTask->callback.pCallback((void*)&taskRet);

END:
	return bRet;
}