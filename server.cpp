#include "server.h"
#include "ui_server.h"
#include <QFileDialog>
#include <QFileInfo>
#include <QMessageBox>

// 初始化UI和FileSender对象，并启动发送线程
Server::Server(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Server),
    fileSenderThread(new QThread(this)),  // 创建新的线程
    fileSender(new FileSender)  // 创建FileSender对象
{
    ui->setupUi(this);

    // 将FileSender对象移动到fileSenderThread线程中执行
    fileSender->moveToThread(fileSenderThread);

    // 连接线程结束信号与FileSender对象的删除槽函数
    connect(fileSenderThread, &QThread::finished, fileSender, &QObject::deleteLater);

    // 连接FileSender的信号到Server的槽函数
    connect(fileSender, &FileSender::progressUpdated, this, &Server::updateProgress);
    connect(fileSender, &FileSender::fileSent, this, &Server::onFileSent);
    connect(fileSender, &FileSender::clientConnected, this, &Server::onClientConnected);

    // 将文件选择信号连接到FileSender的setFile槽函数
    connect(this, &Server::fileSelected, fileSender, &FileSender::setFile);

    // 启动发送线程
    fileSenderThread->start();
}

// 确保在销毁Server对象时正确停止线程
Server::~Server()
{
    fileSenderThread->quit();  // 退出线程的事件循环
    fileSenderThread->wait();  // 等待线程完成执行
    delete ui;
}

// 选择要发送的文件
void Server::on_pushButton_choose_clicked()
{
    // 打开文件选择对话框，获取文件路径
    filePath = QFileDialog::getOpenFileName(this, "打开", "../");
    if (!filePath.isEmpty())
    {
        emit fileSelected(filePath);  // 发射文件选择信号，传递文件路径给FileSender


        QFileInfo info(filePath);
        ui->textEdit->setText("打开文件 " + filePath + "\n");
    }
    else
    {
        qDebug() << "文件路径出错";
    }
}

// 发送文件信息
void Server::on_pushButton_send_clicked()
{
    if (fileSender) {
        // 开始文件发送
        QMetaObject::invokeMethod(fileSender, "sendHeader");
    }
    else {
        QMessageBox::warning(this, "错误", "未连接到客户端！");
    }
}

// 更新发送进度
void Server::updateProgress(qint64 bytesSent, qint64 totalBytes)
{
    int progress = (int)((bytesSent * 1.0 / totalBytes) * 100);
    ui->progressBar->setValue(progress);  // 更新进度条
}

// 处理文件发送完成
void Server::onFileSent()
{
    ui->textEdit->append("文件发送完毕 \n");
    QMessageBox::information(this, "完成", "文件发送完成");
}

// 处理客户端连接信息
void Server::onClientConnected(const QString &clientInfo)
{
    QMessageBox::information(this, "客户端连接", "已连接客户端: " + clientInfo);
}
