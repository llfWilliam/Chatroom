#include "mainwindow.h"
#include <QVBoxLayout>  // Qt 布局管理

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
    client = new ChatClient(this);
    client->connectToServer("127.0.0.1", 12345);

    chatBox = new QTextEdit(this);
    chatBox->setReadOnly(true);

    messageInput = new QLineEdit(this);
    sendButton = new QPushButton("发送", this);

    connect(sendButton, &QPushButton::clicked, this, &MainWindow::onSendClicked);
    connect(client, &ChatClient::messageReceived, this, &MainWindow::onMessageReceived);
    connect(client, &ChatClient::connected, this, &MainWindow::onConnected);
    connect(client, &ChatClient::disconnected, this, &MainWindow::onDisconnected);
    connect(client, &ChatClient::connectionFailed, this, &MainWindow::onConnectionFailed);

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
        client->sendMessage(message);
        messageInput->clear();
    }
}
//收到消息
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
