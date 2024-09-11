#ifndef SERVER_H
#define SERVER_H

#include <QWidget>
#include <QThread>
#include "filesender.h"

namespace Ui {
class Server;
}

class Server : public QWidget
{
    Q_OBJECT

public:
    explicit Server(QWidget *parent = nullptr);
    ~Server();

signals:
    // 信号，用于传递选择的文件路径
    void fileSelected(const QString &filePath);

private slots:
    // 选择文件
    void on_pushButton_choose_clicked();
    // 发送文件头信息
    void on_pushButton_send_clicked();
    // 更新发送进度
    void updateProgress(qint64 bytesSent, qint64 totalBytes);
    // 处理文件发送完成
    void onFileSent();
    // 显示客户端的ip端口信息
    void onClientConnected(const QString &clientInfo);

private:
    Ui::Server *ui;
    QString filePath;           // 要发送的文件路径
    QThread *fileSenderThread;  // 用于文件发送的线程
    FileSender *fileSender;     // 处理文件发送逻辑的对象
};

#endif // SERVER_H
