#pragma once
#ifndef _TASK_2_H_
#define _TASK_2_H_

#include"common.h"
#include"TaskBase.h"

class CTask2 : public CTaskBase
{
public:
	CTask2();
	~CTask2();

	virtual bool Init(void* pvParam);
	virtual bool Excute();

private:
	PTASK_2 m_pTask;
};

#endif // !_TASK_2_H_
