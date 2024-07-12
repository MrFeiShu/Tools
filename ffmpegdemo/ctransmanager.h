#ifndef CTRANSMANAGER_H
#define CTRANSMANAGER_H

#include <QObject>
#include <QList>
#include <QThread>
#include <QSemaphore>
#include "cvideotrans.h"

class CTransManager:public QObject
{
    Q_OBJECT
public:
    CTransManager();
    ~CTransManager();

    // 初始化视频转换
    void InitVideoTrans(int nMaxThread);

    // 添加需要处理的视频信息
    bool AddVideoInfo(const int nIndex, const QString& strInFile, const QString& strOutFile);

    // 清空视频信息
    void ClearVideoTransList();

    // 执行视频转换
    bool ExecVideoTrans();

    // 存储视频转换实例
    QList<CVideoTrans*> m_VideoTransList;
private:

    QSemaphore* m_pSem;

};

#endif // CTRANSMANAGER_H
