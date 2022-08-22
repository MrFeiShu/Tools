#include"common.h"
#include"task1.h"

CTask1::CTask1()
{
	m_pTask = NULL;
}

CTask1::~CTask1()
{
}

bool CTask1::Init(void* pvParam)
{
	bool	bRet = false;

	if (NULL == pvParam)
	{
		printf("invalid param.");
		goto END;
	}

	m_pTask = (PTASK_1)pvParam;

	bRet = true;
END:
	return bRet;
}

bool CTask1::Excute()
{
	bool bRet = false;
	TASK_1_RET taskRet = { 0 };
	char szCmd[STR_LEN * 4] = { 0 };

	printf("task 1 excute, data is %s.\n", m_pTask->data.szMsg);

	FILE* pFile = NULL;
	(void)StringCchPrintfA(szCmd, _countof(szCmd), "ping %s", m_pTask->data.szMsg);

	if (NULL == m_pTask->data.pCallback)
	{
		printf("data callback is null.\n");
		goto END;
	}

	pFile = _popen(szCmd, "rt");

	printf("-----------------------------------------------------------------------\n");

	if (NULL != pFile)
	{
		char szBuf[STR_LEN * 10] = { 0 };
		while (NULL != fgets(szBuf, _countof(szBuf), pFile))
		{
			m_pTask->data.pCallback(szBuf);
		}
	}

	printf("-----------------------------------------------------------------------\n");

	taskRet.header.nType = m_pTask->header.nType;
	taskRet.ret.nRet = 1;

	bRet = m_pTask->callback.pCallback((void*)&taskRet);

END:
	if (NULL != pFile)
	{
		_pclose(pFile);
		pFile = NULL;
	}
	return bRet;
}