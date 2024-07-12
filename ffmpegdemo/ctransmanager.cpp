#include "ctransmanager.h"
#include <qdebug.h>

CTransManager::CTransManager():m_pSem(nullptr)
{

}

CTransManager::~CTransManager()
{
    if(nullptr != m_pSem)
    {
        delete m_pSem;
        m_pSem = nullptr;
    }

    ClearVideoTransList();

}

void CTransManager::InitVideoTrans(int nMaxThread)
{
    if(0 >= nMaxThread || 10 < nMaxThread)
    {
        qDebug("[CTransManager::InitVideoTrans] invlaid param.");
        return;
    }

    m_pSem = new QSemaphore(nMaxThread);
}

bool CTransManager::AddVideoInfo(const int nIndex, const QString& strInFile, const QString& strOutFile)
{
    bool bRet = false;
    CVideoTrans* pVideoTrans = nullptr;

    qDebug("[CTransManager::AddVideoInfo] enter.");
    if( 0 > nIndex || true == strInFile.isEmpty() || true == strOutFile.isEmpty() )
    {
        qDebug("[CTransManager::AddVideoInfo] invalid param.");
        bRet = false;
        goto END;
    }

    pVideoTrans = new CVideoTrans;
    if(nullptr == pVideoTrans)
    {
        qDebug("[CTransManager::AddVideoInfo] new pVideoTrans failed.");
        bRet = false;
        goto END;
    }

    pVideoTrans->InitTrans(m_pSem, nIndex, strInFile, strOutFile);

    m_VideoTransList.append(pVideoTrans);

    qDebug("[CTransManager::AddVideoInfo] videoinfo list size : %d.", (int)m_VideoTransList.size());

    bRet = true;

END:
    qDebug("[CTransManager::AddVideoInfo] leave, bRet=%d.", bRet);

    return bRet;
}

void CTransManager::ClearVideoTransList()
{
    qDebug("[CTransManager::ClearVideoTransList] enter.");

    for(int i = 0; i < m_VideoTransList.size(); i++)
    {
        delete(m_VideoTransList[i]);
    }

    m_VideoTransList.clear();
    qDebug("[CTransManager::ClearVideoTransList] leave.");
}

bool CTransManager::ExecVideoTrans()
{
    bool bRet = false;

    qDebug("[CTransManager::ExecVideoTrans] enter.");

    for (int i = 0; i < m_VideoTransList.size(); i++)
    {
        m_VideoTransList[i]->start();
    }

    bRet = true;
END:
    qDebug("[CTransManager::ExecVideoTrans] leave, bRet = %d.", bRet);
    return bRet;
}
