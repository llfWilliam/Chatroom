#ifndef CLIENT_H
#define CLIENT_H

#include <QTcpSocket>  // TCP 连接模块
#include <QObject>     // Qt 对象基类
#include <QTimer>      // 用于重连计时

class ChatClient : public QObject {
    Q_OBJECT
public:
    explicit ChatClient(QObject *parent = nullptr);
    void connectToServer(const QString &host, int port);  // 连接服务器
    void sendMessage(const QString &message);  // 发送消息
    void disconnectFromServer();  // 断开服务器连接

signals:
    void messageReceived(const QString &message);  // 信号：接收到消息
    void connected();  // 连接成功信号
    void disconnected();  // 连接断开信号
    void connectionFailed(const QString &error);  // 连接失败信号
    void errorOccurred(const QString &error);  // 错误发生信号

private slots:
    void onReadyRead();       // 读取服务器消息
    void onReconnect();       // 处理重连操作
    void onConnected();       // 连接成功处理
    void onDisconnected();    // 处理断开连接
    void onErrorOccurred(QAbstractSocket::SocketError socketError);

private:
    QTcpSocket *socket;  // TCP 连接
    QTimer *reconnectTimer;  // 重连计时器
    int userId = -1;  // 用户唯一 ID
};

#endif // CLIENT_H

