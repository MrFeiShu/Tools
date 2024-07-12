#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDragEnterEvent>
#include <QDropEvent>
#include <QMimeData>
#include <QStringListModel>
#include "ctransmanager.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    void SetOutputPath(const QString& strOutDirPath, bool bSingleFile = true);

private slots:
    void on_pushButtonVideo_clicked();

    void on_pushButtonAudio_clicked();

    void on_pushButtonImage_clicked();

    void on_pushButtonTrans_clicked();

    void on_pushButtonPath_clicked();

    void on_comboBoxFormat_currentTextChanged(const QString &arg1);

    void onNotifyInfo(const int index, const QString& strProgress);

private:
    Ui::MainWindow *ui;
    QStringListModel * m_ListModel;

    CVideoTrans* m_videoTrans;
    CTransManager m_TransManager;

    QString m_strOutDir;
    QString m_strEx;

    QStringList m_ListText;

protected:
    void dragEnterEvent(QDragEnterEvent* event) override;
    void dropEvent(QDropEvent* event) override;
};
#endif // MAINWINDOW_H
