#include "filesender.h"
#include <QFileInfo>
#include <QDebug>

// 初始化成员变量
FileSender::FileSender(QObject *parent)
    : QObject(parent), tcpServer(new QTcpServer(this)), tcpSocket(nullptr), fileSize(0), sendSize(0)
{
    startServer(12345);
    // 当有新的连接时，触发 handleNewConnection 函数
    connect(tcpServer, &QTcpServer::newConnection, this, &FileSender::handleNewConnection);
}

// 析构函数，确保资源正确释放
FileSender::~FileSender()
{
    if (tcpSocket) {
        if (tcpSocket->isOpen()) {
            tcpSocket->disconnect();
            tcpSocket->close();
        }
        delete tcpSocket;
    }
    if (tcpServer->isListening()) {
        tcpServer->close();
    }
    delete tcpServer;
}

// 启动服务器，监听指定端口
void FileSender::startServer(quint16 port)
{
    if (!tcpServer->listen(QHostAddress::Any, port)) {
        qDebug() << "服务器启动失败";
    } else {
        qDebug() << "服务器已启动，监听端口：" << port;
    }
}

// 处理新的客户端连接
void FileSender::handleNewConnection()
{
    tcpSocket = tcpServer->nextPendingConnection();
    QString clientInfo = tcpSocket->peerAddress().toString() + ":" + QString::number(tcpSocket->peerPort());

    // 当客户端有数据可读时，触发 receiveAcknowledgement 函数
    connect(tcpSocket, &QTcpSocket::readyRead, this, &FileSender::receiveAcknowledgement);

    emit clientConnected(clientInfo);  // 发射客户端连接信号
}

// 发送文件头信息
void FileSender::sendHeader()
{
    QString header = fileName + "::" + QString::number(fileSize);
    quint64 writtenBytes = tcpSocket->write(header.toUtf8().data());

    if (writtenBytes <= 0) {
        qDebug() << "Header信息发送失败";
        file.close();
    }
}

// 接收客户端的确认信息，开始发送文件数据
void FileSender::receiveAcknowledgement()
{
    QByteArray buffer = tcpSocket->readAll();

    if (QString(buffer) == fileName) {
        sendFile();  // 开始发送文件
    }
}

// 设置文件路径并准备发送
void FileSender::setFile(const QString &filePath)
{
    file.setFileName(filePath);

    if (!file.open(QIODevice::ReadOnly)) {
        qDebug() << "文件打开失败";
        return;
    }

    QFileInfo info(file);
    fileName = info.fileName();
    fileSize = info.size();
}

// 发送文件数据
void FileSender::sendFile()
{
    qint64 bytesLen = 0;
    char buffer[128 * 1024] = {0};  // 128KB缓冲区
    bytesLen = file.read(buffer, sizeof(buffer));

    while (bytesLen > 0) {
        bytesLen = tcpSocket->write(buffer, bytesLen);
        sendSize += bytesLen;

        // 发射进度更新信号
        emit progressUpdated(sendSize, fileSize);

        bytesLen = file.read(buffer, sizeof(buffer));  // 继续读取文件内容
    }

    if (sendSize == fileSize) {
        emit fileSent();  // 发送文件完成信号
        file.close();
        tcpSocket->disconnect();
        tcpSocket->close();
    }
}
