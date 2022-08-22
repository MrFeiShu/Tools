#include"TasksManager.h"
#include"task1.h"
#include"task2.h"
#include"task3.h"
#include<process.h>

CTasksManager::CTasksManager()
{
}

CTasksManager::~CTasksManager()
{
	while (false == m_taskList.empty())
	{
		CTaskBase* pBase = m_taskList.front();
		if (NULL != pBase)
		{
			delete pBase;
			pBase = NULL;
		}
		m_taskList.pop_front();
	}
}

bool CTasksManager::AddTask(void* pvParam)
{
	bool	bRet = false;
	int		type = TYPE_MAX;
	CTaskBase* pBase = NULL;

	if (NULL == pvParam)
	{
		printf("invalid param.");
		goto END;
	}

	type = ((PDATA_HEADER)pvParam)->nType;

	switch (type)
	{
		case TYPE_TASK_1:
		{
			printf("add task1.");
			pBase = new CTask1;
			pBase->Init(pvParam);
		}
		break;
		case TYPE_TASK_2:
		{
			printf("add task2.");
			pBase = new CTask2;
			pBase->Init(pvParam);
		}
		break;
		case TYPE_TASK_3:
		{
			printf("add task3.");
			pBase = new CTask3;
			pBase->Init(pvParam);
		}
		break;
	default:
		break;
	}
	
	if (NULL == pBase)
	{
		printf("new task failed.");
		goto END;
	}

	m_taskList.push_back(pBase);
	printf("task count %d.\n", (int)m_taskList.size());

	bRet = true;
END:
	return bRet;
}

void CTasksManager::Start()
{
	(void)_beginthreadex(NULL, 0, ExcuteProc, (void*)this, 0, NULL);
}

unsigned int __stdcall CTasksManager::ExcuteProc(void* pvParam)
{
	if (NULL == pvParam)
	{
		printf("invalid param.");
		return 1;
	}

	((CTasksManager*)pvParam)->Excute();

	return 0;
}

void CTasksManager::Excute()
{
#if 0
	TaskList::iterator iter;

	for (iter = m_taskList.begin(); iter != m_taskList.end(); iter++)
	{
		CTaskBase* pBase = (CTaskBase*)*iter;
		(void)pBase->Excute();
	}
#endif // 0

#if 1
	CTaskBase* pBase = NULL;
	do
	{
		printf(">>>>>>excute begin task count is %d.\n", (int)m_taskList.size());
	
		pBase = (CTaskBase*)m_taskList.front();
		if (NULL != pBase)
		{
			pBase->Excute();
			delete pBase;
			pBase = NULL;
		}
		else
		{
			printf("get a NULL pBase.");
		}

		m_taskList.pop_front();

		printf("<<<<<<excute after task count is %d.\n", (int)m_taskList.size());
	} while (false == m_taskList.empty());
#endif // 1

	
}