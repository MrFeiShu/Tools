#include"common.h"
#include"TasksManager.h"

bool RetCallBack(void* pvParam)
{
	bool bRet = false;
	int type = TYPE_MAX;

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
			PTASK_1_RET pRet = (PTASK_1_RET)pvParam;
			printf("task 1 status is %d.\n", pRet->ret.nRet);
		}
		break;
		case TYPE_TASK_2:
		{
			PTASK_2_RET pRet = (PTASK_2_RET)pvParam;
			printf("task 2 status is %d.\n", pRet->ret.nRet);
		}
		break;
		case TYPE_TASK_3:
		{
			PTASK_3_RET pRet = (PTASK_3_RET)pvParam;
			printf("task 3 status is %d.\n", pRet->ret.nRet);
		}
		break;
	default:
		break;
	}

	bRet = true;
END:
	return bRet;
}

bool PrintCallback(const char* szMsg)
{
	if (NULL != szMsg)
	{
		printf(szMsg);
	}

	return true;
}

int main(int argc, char** argv)
{
	int nRet = 0;

	CTasksManager tasks;

#if 0
	TASK_1 task1 = { 0 };
	TASK_2 task2 = { 0 };
	TASK_3 task3 = { 0 };

	task1.header.nType = TYPE_TASK_1;
	task1.callback.pCallback = RetCallBack;
	(void)StringCchCopyA(task1.data.szMsg, _countof(task1.data.szMsg), "www.baidu.com");
	task1.data.pCallback = PrintCallback;

	task2.header.nType = TYPE_TASK_2;
	task2.callback.pCallback = RetCallBack;
	(void)StringCchCopyA(task2.data.szMsg, _countof(task2.data.szMsg), "22222222222222222");

	task3.header.nType = TYPE_TASK_3;
	task3.callback.pCallback = RetCallBack;
	(void)StringCchCopyA(task3.data.szMsg, _countof(task3.data.szMsg), "33333333333333333");

#endif // 0

	TASK_1 task1 = { 0 };
	TASK_1 task2 = { 0 };
	TASK_1 task3 = { 0 };

	task1.header.nType = TYPE_TASK_1;
	task1.callback.pCallback = RetCallBack;
	(void)StringCchCopyA(task1.data.szMsg, _countof(task1.data.szMsg), "www.baidu.com");
	task1.data.pCallback = PrintCallback;


	task2.header.nType = TYPE_TASK_1;
	task2.callback.pCallback = RetCallBack;
	(void)StringCchCopyA(task2.data.szMsg, _countof(task2.data.szMsg), "www.mi.com");
	task2.data.pCallback = PrintCallback;


	task3.header.nType = TYPE_TASK_1;
	task3.callback.pCallback = RetCallBack;
	(void)StringCchCopyA(task3.data.szMsg, _countof(task3.data.szMsg), "www.sohu.com");
	task3.data.pCallback = PrintCallback;

	tasks.AddTask((void*)&task1);
	tasks.AddTask((void*)&task2);
	tasks.AddTask((void*)&task3);

	tasks.Start();

	(void)getchar();

END:
	return nRet;
}