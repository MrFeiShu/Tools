#include"task3.h"

CTask3::CTask3()
{
	m_pTask = NULL;
}

CTask3::~CTask3()
{
}

bool CTask3::Init(void* pvParam)
{
	bool	bRet = false;

	if (NULL == pvParam)
	{
		printf("invalid param.");
		goto END;
	}

	m_pTask = (PTASK_3)pvParam;

	bRet = true;
END:
	return bRet;
}

bool CTask3::Excute()
{
	bool bRet = false;
	TASK_2_RET taskRet = { 0 };

	printf("task 3 excute, data is %s.\n", m_pTask->data.szMsg);

	taskRet.header.nType = m_pTask->header.nType;
	taskRet.ret.nRet = 3;

	bRet = m_pTask->callback.pCallback((void*)&taskRet);

END:
	return bRet;
}