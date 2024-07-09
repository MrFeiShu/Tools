#ifndef CVIDEOTRANS_H
#define CVIDEOTRANS_H

#include <QString>
#include <QThread>

class CVideoTrans : public QThread
{
    Q_OBJECT
public:
    CVideoTrans();

    bool Translate();

    void SetFileInfo(const QString& strInFile, QString& strOutFile);

protected:
    void run() override;

private:
    QString m_strInFile;
    QString m_strOutFile;

signals:
    void NotifyInfo();
};

#endif // CVIDEOTRANS_H
