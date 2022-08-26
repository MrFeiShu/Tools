#ifndef _FILE_COPY_H_
#define _FILE_COPY_H_

class CFileCopy
{
public:
	CFileCopy();
	~CFileCopy();

	BOOL Init(const TCHAR* tzSrcPath, const TCHAR* tzDstPath);

	DWORD Copy();

protected:
private:
	TCHAR m_tzSrcPath[MAX_PATH];
	TCHAR m_tzDstPath[MAX_PATH];
};

#endif
