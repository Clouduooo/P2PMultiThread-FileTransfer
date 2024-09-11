#ifndef FILESENDER_H
#define FILESENDER_H

#include <QObject>
#include <QTcpSocket>
#include <QTcpServer>
#include <QFile>

class FileSender : public QObject
{
    Q_OBJECT

public:
    explicit FileSender(QObject *parent = nullptr);
    ~FileSender();

    // 启动tcp服务器 监听指定端口
    void startServer(quint16 port);

signals:
    // 客户端连接信号
    void clientConnected(const QString &clientInfo);
    // 更新进度的信号
    void progressUpdated(qint64 bytesSent, qint64 totalBytes);
    // 文件发送完成信号
    void fileSent();

public slots:
    // 设置文件路径并准备发送
    void setFile(const QString &filePath);
    // 发送文件数据
    void sendFile();
    // 发送文件头信息
    void sendHeader();

private slots:
    // 处理新连接
    void handleNewConnection();
    // 接收客户端确认
    void receiveAcknowledgement();

private:
    QTcpServer *tcpServer;      // 服务器对象
    QTcpSocket *tcpSocket;      // 与客户端通信的套接字
    QFile file;                 // 要发送的文件对象
    QString fileName;           // 文件名
    qint64 fileSize;            // 文件大小
    qint64 sendSize;            // 已发送的字节数
};

#endif // FILESENDER_H
