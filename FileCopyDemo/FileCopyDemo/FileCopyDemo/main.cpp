#include <Windows.h>
#include <tchar.h>
#include <strsafe.h>
#include "FileCopy.h"
#include <map>
using namespace std;

typedef map<int, TCHAR*> PathMap;

TCHAR * gDstPath[5] = {
	_T("demo\\x86\\SangforCSClient\\Build.xml"),
	_T("demo\\x86\\SangforCore\\Build.xml"),
	_T("demo\\x86\\CSCM\\Build.xml"),
	_T("demo\\x86\\vRemoteApp\\RemoteAppClient\\SfRemoteAppClient\\Build.xml"),
	_T("demo\\x86\\vRemoteApp\\DeskServer\\RVLAgent\\Build.xml")
};

TCHAR * gSrcPath[5] = {
	_T("demo\\Build\\BuildCommon.xml"),
	_T("demo\\Build\\BuildCommon.xml"),
	_T("demo\\Build\\BuildCommon.xml"),
	_T("demo\\Build\\BuildRemoteAppClient.xml"),
	_T("demo\\Build\\BuildRvlAgent.xml")
};



void GetCurPath(TCHAR* ptzPath)
{
	TCHAR tzPathTmp[MAX_PATH] = {0};
	TCHAR* ptzPos = NULL;
	DWORD dwRet = ERROR_SUCCESS;

	dwRet = GetModuleFileName(NULL, tzPathTmp, _countof(tzPathTmp));
	if (0 == dwRet)
	{
		return;
	}

	ptzPos = _tcsrchr(tzPathTmp, _T('\\'));
	if (NULL == ptzPos)
	{
		return;
	}

	(void)StringCchCopyN(ptzPath, MAX_PATH, tzPathTmp, ptzPos - tzPathTmp);
}

int main(int argc, void** argv)
{
	TCHAR tzCurPath[MAX_PATH] = {0};
	PathMap srcPathList, dstPathList;

	GetCurPath(tzCurPath);
	
	for (int i = 0; i < _countof(gSrcPath); i++)
	{
		TCHAR srcPath[MAX_PATH];
		TCHAR dstPath[MAX_PATH];

		(void)StringCchPrintf(srcPath, _countof(srcPath), _T("%s\\%s"), tzCurPath, gSrcPath[i]);
		(void)StringCchPrintf(dstPath, _countof(dstPath), _T("%s\\%s"), tzCurPath, gDstPath[i]);

		CFileCopy fileCopy;
		fileCopy.Init(srcPath, dstPath);

		if (ERROR_SUCCESS != fileCopy.Copy())
		{
			break;
		}
	}

	return 1;
}