#ifndef CLIENT_H
#define CLIENT_H

#include <QWidget>
#include <QThread>

#include "filereceiver.h"

namespace Ui {
class Client;
}

class Client : public QWidget
{
    Q_OBJECT

public:
    explicit Client(QWidget *parent = nullptr);
    ~Client();

signals:
    // 信号，通知FileReceiver开始接收文件
    void startReceiving(const QString &ip, quint16 port);
    // 信号，将目录传递给 FileReceiver
    void directorySelected(const QString &dir);

private slots:
    // 选择保存路径按钮点击时的槽函数
    void on_pushButton_chooseDirectory_clicked();
    // 连接按钮点击时的槽函数
    void on_pushButton_connect_clicked();
    // 暂停按钮点击时的槽函数
    void on_pushButton_pause_clicked();
    // 继续按钮点击时的槽函数
    void on_pushButton_continue_clicked();

    // 文件Header信息成功解析时的槽函数
    void onFileInfoReceived(const QString &info);
    // 更新进度和速度的槽函数
    void updateProgress(int progress, float speed);
    // 文件接收完成时的槽函数
    void onFileReceived(const QString &filePath, float elapsedTime);

private:
    Ui::Client *ui;
    QThread *fileReceiverThread;    // 用于运行FileReceiver的线程
    FileReceiver *fileReceiver;     // 处理文件接收逻辑的对象
};

#endif // CLIENT_H
