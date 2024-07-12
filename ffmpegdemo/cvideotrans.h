#ifndef CVIDEOTRANS_H
#define CVIDEOTRANS_H

#include <QString>
#include <QThread>

typedef struct _st_FrameInfo
{
    unsigned int countPktVideo;
    unsigned int countPktAudio;
    unsigned int countPktSubTitle;
    unsigned int countPktData;
    unsigned int countPktAttachment;
    unsigned int countPktUnkown;
    unsigned int countPktNb;
    unsigned int allFrames;
}FrameInfo, *PFrameInfo;


class CVideoTrans : public QThread
{
    Q_OBJECT
public:
    CVideoTrans();

    bool Translate();

    void SetFileInfo(const QString& strInFile, const QString& strOutFile);

private:
    void CountFrames();

protected:
    void run() override;

private:
    char* m_pszInFile;
    char* m_pszOutFile;

    FrameInfo m_FrameInfo;

signals:
    void NotifyInfo(const QString& strProgress);
};

#endif // CVIDEOTRANS_H
