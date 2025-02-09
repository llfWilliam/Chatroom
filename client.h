#ifndef CLIENT_H
#define CLIENT_H

#include <QTcpSocket>  // TCP 连接模块
#include <QObject>     // Qt 对象基类

class ChatClient : public QObject {
    Q_OBJECT
public:
    explicit ChatClient(QObject *parent = nullptr);
    void connectToServer(const QString &host, quint16 port);  // 连接服务器
    void sendMessage(const QString &message);  // 发送消息

private slots:
    void onReadyRead();       // 读取服务器消息
    void onDisconnected();    // 处理断开连接

private:
    QTcpSocket *socket;  // TCP 连接
};

#endif // CLIENT_H
