#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <qdebug.h>
#include <QFileDialog>
#include <QFileInfo>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , listModel(nullptr)
    , m_videoTrans(nullptr)
{
    ui->setupUi(this);
    ui->listViewItem->setAcceptDrops(true);

    ui->comboBoxFormat->addItem(tr("mkv"));
    ui->comboBoxFormat->addItem(tr("mp4"));
    ui->comboBoxFormat->addItem(tr("avi"));
    ui->comboBoxFormat->addItem(tr("rmvb"));

    m_videoTrans = new CVideoTrans;
    connect(m_videoTrans, SIGNAL(NotifyInfo()), this, SLOT(onNotifyInfo()));
}

MainWindow::~MainWindow()
{
    delete ui;
    delete listModel;
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

void MainWindow::dragEnterEvent(QDragEnterEvent* event) {
    if (event->mimeData()->hasUrls()) {
        event->acceptProposedAction();
    }
}

void MainWindow::dropEvent(QDropEvent* event) {
    const QMimeData* mimeData = event->mimeData();
    if (mimeData->hasUrls()) {
        QList<QUrl> urlList = mimeData->urls();
        QStringList list;
        for (const QUrl& url : urlList) {
            list.append(url.toLocalFile());
            qDebug() << u8"拖拽的文件路径：" << url.toLocalFile();
        }

        listModel = new QStringListModel(list);
        ui->listViewItem->setModel(listModel);

        m_strInFile = list[0];
    }
}

void MainWindow::on_pushButtonTrans_clicked()
{
    QString OutFile;

    if(true == m_strInFile.isEmpty() || true == m_strOutFileBase.isEmpty())
    {
        qDebug("[ty] invalid path.");
        return;
    }

    OutFile = m_strOutFileBase + m_strEx;
    qDebug()<<"InFile: "<< m_strInFile<<"OutFile:"<<OutFile;

    if(nullptr == m_videoTrans)
    {
        qDebug("[ty] m_videoTrans is nullptr.");
        return;
    }

    m_videoTrans->SetFileInfo(m_strInFile, OutFile);

    m_videoTrans->start();
}


void MainWindow::on_pushButtonPath_clicked()
{
    QString selectDir = QFileDialog::getExistingDirectory();
    qDebug() << "Dir Path:" << selectDir;

    if(false == m_strInFile.isEmpty())
    {
        QFileInfo fileInfo(m_strInFile);
        qDebug()<<tr("base name:")<<fileInfo.baseName();

        m_strOutFileBase = selectDir;
        qDebug()<<tr("output file path 1: ")<<m_strOutFileBase;

        if(false == m_strOutFileBase.endsWith(tr("/")))
        {
            m_strOutFileBase += tr("/");
            qDebug()<<tr("output file path 2: ")<<m_strOutFileBase;
        }

        m_strOutFileBase += fileInfo.baseName();
        qDebug()<<tr("output file path 3: ")<<m_strOutFileBase;
        m_strOutFileBase += tr(".");
        qDebug()<<tr("output file path 4: ")<<m_strOutFileBase;

        QString OutFile = m_strOutFileBase + m_strEx;

        ui->lineEditOutputPath->setText(OutFile);
    }
}


void MainWindow::on_comboBoxFormat_currentTextChanged(const QString &arg1)
{
    qDebug()<<tr("text:")<<arg1;
    m_strEx = arg1;

    if(false == m_strOutFileBase.isEmpty() && false == m_strEx.isEmpty())
    {
        QString OutFile = m_strOutFileBase + m_strEx;
        ui->lineEditOutputPath->setText(OutFile);
    }
}

void MainWindow::onNotifyInfo()
{
    qDebug("[ty] MainWindow::onNotifyInfo enter.");
}
