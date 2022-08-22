#ifndef _PARSE_CFG_H_
#define _PARSE_CFG_H_

#include "tinyxml2.h"
#include <list>
#include <map>
using namespace std;

typedef struct _st_TASK_INFO
{
	char	szName[MAX_PATH];
	char	szPath[MAX_PATH];
	BOOL	bEnable;
	int		nLevel;
	int		nTaskID;
	int		nParentID;
}TASK_INFO, *PTASK_INFO;

typedef list<PTASK_INFO> TaskInfoList;
//typedef map<int, *TaskInfoList> TaskInfoMap;

void ParseCfgTest();

#endif
