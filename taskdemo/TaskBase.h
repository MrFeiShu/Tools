#pragma once
#ifndef _TASK_H_
#define _TASK_H_

#include"common.h"

class CTaskBase
{
public:
	CTaskBase();
	~CTaskBase();

	virtual bool Init(void* pvParam) = 0;
	virtual bool Excute() = 0;
private:

};

#endif // !_TASK_H_
