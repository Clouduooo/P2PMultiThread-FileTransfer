#ifndef FILERECEIVER_H
#define FILERECEIVER_H

#include <QObject>
#include <QTcpSocket>
#include <QFile>
#include <QElapsedTimer>

class FileReceiver : public QObject
{
    Q_OBJECT
public:
    explicit FileReceiver(QObject *parent = nullptr);
    ~FileReceiver();

    // 启动接收文件，连接到指定的IP和端口
    void startReceiving(const QString &ip, quint16 port);

signals:
    // 信号，通知UI显示文件信息
    void fileInfoReceived(const QString &info);
    // 信号，通知UI更新进度和速度
    void progressUpdated(int progress, float speed);
    // 信号，通知UI文件接收完成
    void fileReceived(const QString &filePath, float elapsedTime);

public slots:
    // 槽函数，处理接收文件的数据
    void receiveFile();
    // 槽函数，暂停接收
    void pauseReceiving();
    // 槽函数，继续接收
    void continueReceiving();
    // 槽函数，接收文件保存路径
    void setDirectory(const QString &dir);

private:
    QFile file;
    QTcpSocket *tcpSocket;
    QString fileName;           // 接收文件的保存路径
    qint64 fileSize;            // 接收文件的实际总字节数
    qint64 receiveSize;         // 已经接收的字节数
    QElapsedTimer receiveClock; // 计算文件接收时间的计时器
    int last_time = 0;          // 上一次记录的时间（毫秒数）
    bool isfirstInfo = true;    // 是否为第一批数据——文件的Header信息
    bool isPaused = false;      // 是否暂停
    QString dir;
};

#endif // FILERECEIVER_H
