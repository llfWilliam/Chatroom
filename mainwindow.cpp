#include "mainwindow.h"
#include <QVBoxLayout>  // Qt 布局管理
#include <QThread>      // 用于创建和管理线程

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
    // 创建客户端对象
    client = new ChatClient(this);

    // 连接到服务器
    client->connectToServer("127.0.0.1", 12345);

    // 初始化界面控件
    chatBox = new QTextEdit(this);
    chatBox->setReadOnly(true);

    messageInput = new QLineEdit(this);
    sendButton = new QPushButton("发送", this);

    // 连接信号和槽
    connect(sendButton, &QPushButton::clicked, this, &MainWindow::onSendClicked);
    connect(client, &ChatClient::messageReceived, this, &MainWindow::onMessageReceived);
    connect(client, &ChatClient::connected, this, &MainWindow::onConnected);
    connect(client, &ChatClient::disconnected, this, &MainWindow::onDisconnected);
    connect(client, &ChatClient::connectionFailed, this, &MainWindow::onConnectionFailed);
    connect(client, &ChatClient::errorOccurred, this, &MainWindow::onErrorOccurred);

    // 布局设置
    QVBoxLayout *layout = new QVBoxLayout();
    layout->addWidget(chatBox);
    layout->addWidget(messageInput);
    layout->addWidget(sendButton);

    QWidget *centralWidget = new QWidget(this);
    centralWidget->setLayout(layout);
    setCentralWidget(centralWidget);
}

// 发送消息
void MainWindow::onSendClicked() {
    QString message = messageInput->text();
    if (!message.isEmpty()) {
        client->sendMessage(message);  // 发送消息
        messageInput->clear();  // 清空输入框
    }
}

// 收到消息
void MainWindow::onMessageReceived(const QString &message) {
    chatBox->append(message);  // 显示服务器返回的消息
}

// 处理连接成功
void MainWindow::onConnected() {
    chatBox->append("[系统] 成功连接到服务器!");
}

// 处理断开连接
void MainWindow::onDisconnected() {
    chatBox->append("[系统] 服务器断开连接，正在重试...");
}

// 处理连接失败
void MainWindow::onConnectionFailed(const QString &error) {
    chatBox->append("[系统] 连接失败: " + error);
}

// 处理错误发生
void MainWindow::onErrorOccurred(const QString &error) {
    chatBox->append("[系统] 错误: " + error);
}
