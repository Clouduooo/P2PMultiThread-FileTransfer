#include <QVBoxLayout>

#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , client(new Client(this))
    , server(new Server(this))
{
    ui->setupUi(this);

    setupUI();

    setupStyleSheet();
}

// 设置主窗口UI
void MainWindow::setupUI()
{
    // 创建主窗口布局容纳client和server的ui
    // 创建中央widget容器
    QWidget* centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);
    // 创建垂直布局
    QVBoxLayout* layout = new QVBoxLayout();
    // 添加client和server ui
    layout->addWidget(client);
    layout->addWidget(server);
    // 将layout加入到中央widget中
    centralWidget->setLayout(layout);
    // 设置主窗口标题
    setWindowTitle("P2P文件传输系统");
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setupStyleSheet()
{
    QString styleSheet = R"(
        /* 全局设置 */
        QWidget {
            background-color: #F0F0F5;
            color: #333333;
            font-family: "Helvetica Neue", Helvetica, Arial, sans-serif;
            font-size: 13px;
        }

        /* QPushButton 样式 */
        QPushButton {
            background-color: #5A9BD5;  /* 使用柔和的蓝色背景 */
            color: white;
            border: none;
            border-radius: 5px;
            padding: 8px 16px;
            font-size: 14px;
        }

        QPushButton:hover {
            background-color: #4A8CC7;  /* 略微深一点的蓝色 */
        }

        QPushButton:pressed {
            background-color: #3A7BB7;  /* 按下时的深蓝色 */
        }

        /* QLabel 样式 */
        QLabel {
            color: #333333;
            font-size: 13px;
        }

        /* QLineEdit 样式 */
        QLineEdit {
            background-color: white;
            border: 1px solid #CCCCCC;
            border-radius: 4px;
            padding: 6px;
            font-size: 13px;
            color: #333333;
        }

        QLineEdit:focus {
            border-color: #5A9BD5;
        }

        /* QTextEdit 样式 */
        QTextEdit {
            background-color: white;
            border: 1px solid #CCCCCC;
            border-radius: 4px;
            padding: 6px;
            font-size: 13px;
            color: #333333;
        }

        /* QProgressBar 样式 */
        QProgressBar {
            background-color: #E0E0E0;
            border: 1px solid #CCCCCC;
            border-radius: 5px;
            text-align: center;
            color: #333333;
            font-size: 12px;
        }

        QProgressBar::chunk {
            background-color: #5A9BD5;
            border-radius: 5px;
        }

        /* QMainWindow 样式 */
        QMainWindow {
            background-color: #F0F0F5;
        }

        QStatusBar {
            background-color: #F0F0F5;
            border-top: 1px solid #CCCCCC;
        }

        /* 其他细节 */
        QWidget[objectName="centralwidget"] {
            padding: 10px;
        }

        QVBoxLayout, QHBoxLayout {
            spacing: 10px;
        }
    )";


    this->setStyleSheet(styleSheet);
}
