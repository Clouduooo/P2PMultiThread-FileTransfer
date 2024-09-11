#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "client.h"
#include "server.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    // 设置UI布局
    void setupUI();
    // 设置QSS样式表等美化内容
    void setupStyleSheet();

private:
    Ui::MainWindow *ui;
    Client* client;
    Server* server;
};
#endif // MAINWINDOW_H
