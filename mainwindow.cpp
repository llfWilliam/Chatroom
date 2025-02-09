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
