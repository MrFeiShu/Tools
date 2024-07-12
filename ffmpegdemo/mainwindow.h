#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDragEnterEvent>
#include <QDropEvent>
#include <QMimeData>
#include <QStringListModel>
#include "cvideotrans.h"

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
    void SetOutputPath(const QString& strOutDirPath);

private slots:
    void on_pushButtonVideo_clicked();

    void on_pushButtonAudio_clicked();

    void on_pushButtonImage_clicked();

    void on_pushButtonTrans_clicked();

    void on_pushButtonPath_clicked();

    void on_comboBoxFormat_currentTextChanged(const QString &arg1);

    void onNotifyInfo(const QString& strProgress);

private:
    Ui::MainWindow *ui;
    QStringListModel * m_ListModel;

    CVideoTrans* m_videoTrans;

    QString m_strInFile;
    //QString m_strOutFile;

    QString m_strOutFileBase;

    QString m_strEx;

    QStringList m_ListText;

protected:
    void dragEnterEvent(QDragEnterEvent* event) override;
    void dropEvent(QDropEvent* event) override;
};
#endif // MAINWINDOW_H
