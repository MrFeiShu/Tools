#pragma once
#ifndef _TASK_3_H_
#define _TASK_3_H_

#include"common.h"
#include"TaskBase.h"

class CTask3 : public CTaskBase
{
public:
	CTask3();
	~CTask3();

	virtual bool Init(void* pvParam);
	virtual bool Excute();

private:
	PTASK_3 m_pTask;
};

#endif // !_TASK_3_H_
