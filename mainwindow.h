#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTextEdit>
#include <QLineEdit>
#include <QPushButton>
#include "client.h"  // 引入 ChatClient 类

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = nullptr);

private slots:
    void onSendClicked();  // 发送按钮事件
    void onMessageReceived(const QString &message);  // 接收消息
    void onConnected();  // 连接成功
    void onDisconnected();  // 连接断开
    void onConnectionFailed(const QString &error);  // 连接失败
    void onErrorOccurred(const QString &error);  // 处理错误

private:
    ChatClient *client;  // 客户端对象
    QTextEdit *chatBox;  // 显示聊天内容
    QLineEdit *messageInput;  // 输入框
    QPushButton *sendButton;  // 发送按钮
};

#endif // MAINWINDOW_H
