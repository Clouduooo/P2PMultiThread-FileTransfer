#include <QFileDialog>
#include <QMessageBox>
#include <QDebug>

#include "filereceiver.h"

// 初始化成员变量，并设置套接字读取信号的连接
FileReceiver::FileReceiver(QObject *parent)
    : QObject(parent), isfirstInfo(true), isPaused(false), last_time(0)
{
    // 初始化TCP套接字
    tcpSocket = new QTcpSocket(this);
    // 当有数据可读时，触发receiveFile函数
    connect(tcpSocket, &QTcpSocket::readyRead, this, &FileReceiver::receiveFile);
}

// 确保套接字断开连接并关闭
FileReceiver::~FileReceiver()
{
    if (tcpSocket->isOpen()) {
        tcpSocket->disconnectFromHost();
        tcpSocket->close();
    }
    disconnect(tcpSocket, &QTcpSocket::readyRead, this, &FileReceiver::receiveFile);
}

// 开始接收文件，连接到指定的服务器
void FileReceiver::startReceiving(const QString &ip, quint16 port)
{
    // 连接到服务器
    tcpSocket->connectToHost(ip, port);
}

// 从UI接收文件保存路径
void FileReceiver::setDirectory(const QString &dir)
{
    // 这里接收到目录路径，可以在接收文件时使用
    this->dir = dir;
}

// 暂停接收文件
void FileReceiver::pauseReceiving()
{
    isPaused = true;
}

// 继续接收文件
void FileReceiver::continueReceiving()
{
    isPaused = false;
    receiveFile();  // 继续从套接字中读取数据
}

// 处理接收到的数据
void FileReceiver::receiveFile()
{
    // 如果处于暂停状态，则不处理数据
    if (isPaused) return;

    // 读取套接字中的所有数据
    QByteArray buffer = tcpSocket->readAll();

    // 如果是文件头信息（第一次接收的数据）
    if(isfirstInfo)
    {
        isfirstInfo = false;
        // 从接收到的头信息中提取文件名和文件大小
        fileName = dir.append("/").append(QString(buffer).section("::", 0, 0));
        fileSize = QString(buffer).section("::", 1, 1).toInt();
        QString info = tr("文件名称：").append(QString(buffer).section("::", 0, 0)).append("\n")
                           .append("保存路径：").append(dir).append("\n")
                           .append("文件大小：").append(QString::number(fileSize / 1024.0 / 1024.0, 'f', 2)).append("Mb");
        emit fileInfoReceived(info);  // 发送信号，通知UI显示文件信息
        receiveSize = 0;  // 初始化已接收的字节数
        // 发送确认信息给服务器，表示已经接收到文件头信息
        tcpSocket->write(QString(buffer).section("::", 0, 0).toUtf8().data());
        // 打开文件准备写入数据
        file.setFileName(fileName);
        if(!file.open(QIODevice::WriteOnly))
        {
            qDebug() << "write only error";  // 如果文件无法打开，输出错误信息
            return;
        }
        receiveClock.start();  // 开始计时
    }
    else
    {
        // 接收并写入文件数据
        qint64 tempBytes = 0;
        static qint64 receiveCount = 0;  // 统计接收次数
        receiveCount++;
        qint64 bytesLen = file.write(buffer);  // 将数据写入文件
        receiveSize += bytesLen;  // 更新已接收的字节数
        tempBytes += bytesLen;

        // 计算接收进度百分比
        int progress = (int)((receiveSize * 1.0) / (fileSize * 1.0) * 100);

        // 每接收10次更新一次进度条
        if(receiveCount % 10 == 0)
        {
            float speed;
            QTime current_time = QTime::currentTime();  // 获取当前时间
            int msec = current_time.msec();  // 获取当前时间的毫秒部分
            if(last_time != 0)  // 如果不是第一次计时
            {
                // 计算接收速度（MB/s）
                if(msec < last_time)
                    speed = tempBytes * 1000.0 / ((msec + 1000 - last_time) * 1024.0 * 1024.0);
                else
                    speed = tempBytes * 1000.0 / ((msec - last_time) * 1024.0 * 1024.0);
            }
            last_time = msec;  // 更新最后的时间
            tempBytes = 0;  // 重置临时字节数
            emit progressUpdated(progress, speed);  // 发送更新进度的信号
        }

        // 如果接收完成
        if(receiveSize == fileSize)
        {
            file.close();  // 关闭文件
            emit fileReceived(fileName, receiveClock.elapsed() / 1000.0);  // 发送接收完成信号
            tcpSocket->disconnectFromHost();  // 断开连接
            tcpSocket->close();
            isfirstInfo = true;  // 重置状态，准备接收下一个文件
        }
    }
}
