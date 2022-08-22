#pragma once
#ifndef _TASK_1_H_
#define _TASK_1_H_

#include"common.h"
#include"TaskBase.h"

class CTask1 : public CTaskBase
{
public:
	CTask1();
	~CTask1();

	virtual bool Init(void* pvParam);
	virtual bool Excute();

private:
	PTASK_1 m_pTask;
};


#endif // !_TASK_1_H_
