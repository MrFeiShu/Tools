#include <Windows.h>
#include "FileCopy.h"
#include <tchar.h>
#include <strsafe.h>

CFileCopy::CFileCopy()
{
	ZeroMemory(m_tzSrcPath, sizeof(m_tzSrcPath));
	ZeroMemory(m_tzDstPath, sizeof(m_tzDstPath));
}

CFileCopy::~CFileCopy()
{
	ZeroMemory(m_tzSrcPath, sizeof(m_tzSrcPath));
	ZeroMemory(m_tzDstPath, sizeof(m_tzDstPath));
}

BOOL CFileCopy::Init(const TCHAR* tzSrcPath, const TCHAR* tzDstPath)
{
	(void)StringCchCopy(m_tzSrcPath, _countof(m_tzSrcPath), tzSrcPath);
	(void)StringCchCopy(m_tzDstPath, _countof(m_tzDstPath), tzDstPath);

	return TRUE;
}

DWORD CFileCopy::Copy()
{
	DWORD dwRet = ERROR_SUCCESS;

	if (FALSE == CopyFile(m_tzSrcPath, m_tzDstPath, FALSE))
	{
		dwRet = GetLastError();
	}

	return dwRet;
}