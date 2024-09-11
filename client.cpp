#include "client.h"
#include "ui_client.h"
#include <QMessageBox>
#include <QFileDialog>

// 初始化UI和FileReceiver对象，并启动接收线程
Client::Client(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Client),
    fileReceiverThread(new QThread(this)),  // 创建新的线程
    fileReceiver(new FileReceiver)  // 创建FileReceiver对象
{
    ui->setupUi(this);

    // 将FileReceiver对象移动到fileReceiverThread线程中执行
    fileReceiver->moveToThread(fileReceiverThread);

    // 连接线程结束信号与FileReceiver对象的删除槽函数
    connect(fileReceiverThread, &QThread::finished, fileReceiver, &QObject::deleteLater);

    // 连接UI信号与FileReceiver槽函数
    connect(this, &Client::startReceiving, fileReceiver, &FileReceiver::startReceiving);
    connect(fileReceiver, &FileReceiver::progressUpdated, this, &Client::updateProgress);
    connect(fileReceiver, &FileReceiver::fileReceived, this, &Client::onFileReceived);
    connect(fileReceiver, &FileReceiver::fileInfoReceived, this, &Client::onFileInfoReceived);

    // 连接信号到 FileReceiver 的槽函数
    connect(this, &Client::directorySelected, fileReceiver, &FileReceiver::setDirectory);

    // 启动接收线程
    fileReceiverThread->start();
}

// 确保在销毁Client对象时正确停止线程
Client::~Client()
{
    fileReceiverThread->quit();  // 退出线程的事件循环
    fileReceiverThread->wait();  // 等待线程完成执行
    delete ui;  // 删除UI对象
}

// 处理“连接”按钮点击事件，开始连接到服务器并接收文件
void Client::on_pushButton_connect_clicked()
{
    QString ip = ui->lineEdit_ip->text();  // 获取用户输入的IP地址
    quint16 port = ui->lineEdit_port->text().toInt();  // 获取用户输入的端口号
    emit startReceiving(ip, port);  // 发射信号，通知FileReceiver开始连接并接收文件
}

// 处理“暂停”按钮点击事件，暂停文件接收
void Client::on_pushButton_pause_clicked()
{
    // fileReceiver->pauseReceiving();  // 调用FileReceiver的暂停接收函数
    QMetaObject::invokeMethod(fileReceiver, "pauseReceiving");
}

// 处理“继续”按钮点击事件，继续文件接收
void Client::on_pushButton_continue_clicked()
{
    // fileReceiver->continueReceiving();  // 调用FileReceiver的继续接收函数
    QMetaObject::invokeMethod(fileReceiver, "continueReceiving");
}

void Client::on_pushButton_chooseDirectory_clicked()
{
    QString dir = QFileDialog::getExistingDirectory(this, "选择文件夹", "./");
    if (!dir.isEmpty()) {
        emit directorySelected(dir);  // 发射信号，将目录传递给 FileReceiver
    }
}

// 槽函数，更新UI中的进度条和速度显示
void Client::updateProgress(int progress, float speed)
{
    ui->progressBar->setValue(progress);  // 更新进度条的值
    ui->label_speed->setText(QString::number(speed, 'f', 2).append("Mb/s"));  // 显示当前接收速度
}

// 槽函数，当接收到文件头部信息时，显示文件传输信息
void Client::onFileInfoReceived(const QString &info)
{
    ui->textEdit_filemsg->setText(info);  // 在UI中显示文件传输信息
}

// 槽函数，当文件接收完成时，显示完成信息并更新UI
void Client::onFileReceived(const QString &filePath, float elapsedTime)
{
    // 在文本编辑器中显示文件完成信息
    ui->textEdit_filemsg->append("文件接收完成\n保存路径: " + filePath + "\n用时: " + QString::number(elapsedTime, 'f', 1) + "s");

    ui->progressBar->setValue(100);  // 设置进度条为100%
    ui->label_speed->setText("0.00 Mb/s");  // 重置速度显示为0

    // 弹出信息框提示用户文件接收完成
    QMessageBox::information(this, "完成", "文件接收完成");
}
