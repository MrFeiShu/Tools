#include "stdafx.h"
#include "ParseCfg.h"
#include <strsafe.h>

#define ATTRIUTE_NAME	"name"
#define ATTRIUTE_PATH	"path"
#define ATTRIUTE_ENABLE	"enable"
#define ATTRIUTE_LEVEL	"level"

#define ATTRIBUTE_ENABLE_TRUE_VALUE		"true"
#define ATTRIBUTE_ENABLE_FALSE_VALUE	"false"



using namespace tinyxml2;

#define TEMP_CFG_XML "E:\\tempconf.xml"

PTASK_INFO GetTaskInfo(tinyxml2::XMLElement* ptrNode, PTASK_INFO pParentTask)
{
	static int nID = 0; // 从1开始，0表示没有父任务
	PTASK_INFO pInfo = new TASK_INFO;

	if (NULL == pInfo)
	{
		printf("new TASK_INFO faile.");
		goto END;
	}

	ZeroMemory(pInfo, sizeof(TASK_INFO));

	// 设置任务名称
	(void)StringCchCopyA(pInfo->szName, _countof(pInfo->szName), ptrNode->Attribute(ATTRIUTE_NAME));

	// 设置任务路径
	(void)StringCchCopyA(pInfo->szPath, _countof(pInfo->szPath), ptrNode->Attribute(ATTRIUTE_PATH));

	// 设置任务使能
	pInfo->bEnable = FALSE;
	if (0 == stricmp(ptrNode->Attribute(ATTRIUTE_ENABLE), ATTRIBUTE_ENABLE_TRUE_VALUE))
	{
		pInfo->bEnable = TRUE;
	}

	// 设置任务级别
	pInfo->nLevel = atoi(ptrNode->Attribute(ATTRIUTE_LEVEL));

	// 设置任务ID
	pInfo->nTaskID = nID;

	// 设置父任务ID
	if (NULL == pParentTask)
	{
		pInfo->nParentID = 0;
	}
	else
	{
		pInfo->nParentID = pParentTask->nTaskID;
	}

	{
		char szMsg[300] = {0};
		(void)StringCchPrintfA(szMsg, _countof(szMsg), "[TASK] [%s - %s - %s - %d - %d - %d].", 
			pInfo->szName, 
			pInfo->szPath, 
			pInfo->bEnable ? ATTRIBUTE_ENABLE_TRUE_VALUE : ATTRIBUTE_ENABLE_FALSE_VALUE, 
			pInfo->nLevel, 
			pInfo->nTaskID, 
			pInfo->nParentID);
		OutputDebugStringA(szMsg);
	}

	nID++;
END:
	return pInfo;
}

void EnumCfg(tinyxml2::XMLElement* ptrNode, tinyxml2::XMLElement* ptrParentNode)
{

}

void GetXmlCfg(tinyxml2::XMLElement* ptrNode, PTASK_INFO pParentInfo)
{
	tinyxml2::XMLElement* ptrNext = NULL;
	PTASK_INFO pInfo = NULL;

	if (NULL == ptrNode)
	{
		printf("invalid param.");
		goto END;
	}

	pInfo = GetTaskInfo(ptrNode, pParentInfo);
	if (NULL == pInfo)
	{
		printf("GetTaskInfo failed.");
		goto END;
	}

	if (false == ptrNode->NoChildren())
	{
		tinyxml2::XMLElement* ptrChildNode = ptrNode->FirstChildElement();
		
		while(NULL != ptrChildNode)
		{
			GetXmlCfg(ptrChildNode, pInfo);
			ptrChildNode = ptrChildNode->NextSiblingElement();
		}
	}

END:
	return;
}

void ParseCfgTest()
{
	tinyxml2::XMLDocument doc;
	tinyxml2::XMLElement* pRoot = NULL;
	tinyxml2::XMLElement* ptrNode = NULL;

	int nRet = doc.LoadFile(TEMP_CFG_XML);
	if (XML_SUCCESS != nRet)
	{
		printf("load cfg failed with error %d.", nRet);
		goto END;
	}

	pRoot = doc.RootElement();
	if (NULL == pRoot)
	{
		printf("get cfg root node failed.");
		goto END;
	}

	GetXmlCfg(pRoot, NULL);

END:
	return;
}