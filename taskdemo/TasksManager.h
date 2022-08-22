#pragma once
#ifndef _TASKS_MANAGER_H_
#define _TASKS_MANAGER_H_

#include"common.h"
#include"TaskBase.h"
#include<list>
using namespace std;

typedef list<CTaskBase*> TaskList;

class CTasksManager
{
public:
	CTasksManager();
	~CTasksManager();

	bool AddTask(void* pvParam);
	void Start();

	static unsigned int __stdcall ExcuteProc(void* pvParam);
	void Excute();

private:
	TaskList m_taskList;
};

#endif // !_TASKS_MANAGER_H_
