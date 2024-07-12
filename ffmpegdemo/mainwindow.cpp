#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <qdebug.h>
#include <QFileDialog>
#include <QFileInfo>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , m_ListModel(nullptr)
    , m_videoTrans(nullptr)
{
    ui->setupUi(this);
    ui->listViewItem->setAcceptDrops(true);

    ui->comboBoxFormat->addItem(tr("mkv"));
    ui->comboBoxFormat->addItem(tr("mp4"));
    ui->comboBoxFormat->addItem(tr("avi"));
    ui->comboBoxFormat->addItem(tr("rmvb"));

    m_videoTrans = new CVideoTrans;
}

MainWindow::~MainWindow()
{
    delete ui;
    delete m_ListModel;
}

void MainWindow::on_pushButtonVideo_clicked()
{
    qDebug("[ty]on_pushButtonVideo_clicked enter.");

}


void MainWindow::on_pushButtonAudio_clicked()
{
    qDebug("[ty]on_pushButtonAudio_clicked enter.");

}


void MainWindow::on_pushButtonImage_clicked()
{
    qDebug("[ty]on_pushButtonImage_clicked enter.");

}

void MainWindow::dragEnterEvent(QDragEnterEvent* event)
{
    if (event->mimeData()->hasUrls())
    {
        event->acceptProposedAction();
    }
}

void MainWindow::dropEvent(QDropEvent* event)
{
    const QMimeData* mimeData = event->mimeData();
    if (mimeData->hasUrls())
    {
        m_ListText.clear();

        QList<QUrl> urlList = mimeData->urls();
        for (const QUrl& url : urlList)
        {
            m_ListText.append(url.toLocalFile());
            qDebug() << u8"拖拽的文件路径：" << url.toLocalFile();
        }
    }

    m_ListModel = new QStringListModel(m_ListText);
    ui->listViewItem->setModel(m_ListModel);

    // 单个文件，默认输出目录为当前文件的目录
    // 多个文件，则强制用户选择输出目录
    if(m_ListText.size() == 1)
    {
        QString strInFile = m_ListText[0];
        QFileInfo info(strInFile);

        SetOutputPath(info.dir().absolutePath());
    }
}

void MainWindow::on_pushButtonTrans_clicked()
{
    QString strOutFile;
    QString strInFile;

    m_TransManager.InitVideoTrans(2);

    for(int i = 0; i < m_ListText.size(); i++)
    {
        strInFile = m_ListText[i];
        QFileInfo info(strInFile);

        strOutFile = m_strOutDir;
        if(false == strOutFile.endsWith(tr("/")))
        {
            strOutFile += tr("/");
        }
        strOutFile += info.baseName();
        strOutFile += tr(".");
        strOutFile += m_strEx;

        (void)m_TransManager.AddVideoInfo(i, strInFile, strOutFile);
    }

    for (int i = 0; i < m_TransManager.m_VideoTransList.size(); i++)
    {
        connect(m_TransManager.m_VideoTransList[i], SIGNAL(NotifyInfo(int, QString)), this, SLOT(onNotifyInfo(int, QString)));
    }

    (void)m_TransManager.ExecVideoTrans();
}

void MainWindow::on_pushButtonPath_clicked()
{
    QString selectDir = QFileDialog::getExistingDirectory();
    qDebug() << "Dir Path:" << selectDir;

    // 只有一个待转换的视频文件，则显示完整输出路径；否则只显示输出目录
    SetOutputPath(selectDir, (m_ListText.size() == 1) ? true : false);
}

void MainWindow::on_comboBoxFormat_currentTextChanged(const QString &arg1)
{
    qDebug()<<tr("text:")<<arg1;
    m_strEx = arg1;

    if(false == m_strOutDir.isEmpty() && false == m_strEx.isEmpty())
    {
        QString OutFile = m_strOutDir + m_strEx;
        ui->lineEditOutputPath->setText(OutFile);
    }
}

void MainWindow::onNotifyInfo(const int index, const QString& strProgress)
{
    qDebug("[ty] MainWindow::onNotifyInfo enter.");

    // 获取listview的内容并修改
    QString strText;
    QModelIndex qIndex = m_ListModel->index(index, 0);
    if(qIndex.isValid())
    {
        strText = m_ListText[qIndex.row()];
        strText += tr(" [");
        strText += strProgress;
        strText += tr("]");

        m_ListModel->setData(qIndex, strText, Qt::DisplayRole);
    }
}

void MainWindow::SetOutputPath(const QString& strOutDirPath, bool bSingleFile/* = true*/)
{
    QString OutFile;

    m_strOutDir = strOutDirPath;

    if(true == bSingleFile)
    {
        // 单个文件显示完整的路径名
        QFileInfo fileInfo(m_ListText[0]);
        qDebug()<<tr("base name:")<<fileInfo.baseName();

        OutFile = m_strOutDir;

        if(false == m_strOutDir.endsWith(tr("/")))
        {
            OutFile += tr("/");
        }
        OutFile += fileInfo.baseName();
        OutFile += tr(".");
        OutFile += m_strEx;
    }
    else
    {
        // 批量处理则只显示输出目录
        OutFile = strOutDirPath;
    }

    ui->lineEditOutputPath->setText(OutFile);
}
