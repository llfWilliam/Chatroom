#ifndef SERVER_H
#define SERVER_H

#include <QTimer>      // 为定时做准备
#include <QTcpServer>  // TCP 服务器模块
#include <QTcpSocket>  // TCP 通信模块
#include <QMap>        // 用于存储多个客户端连接
#include <QObject>     // Qt 对象基类

class ChatServer : public QTcpServer {
    Q_OBJECT
public:
    explicit ChatServer(QObject *parent = nullptr);
    void startServer(quint16 port);  // 启动服务器

private slots:
    void onClientMessageReceived(int userId, const QString &message);  // 处理客户端发来的消息
    void onClientDisconnected(int userId);  // 处理客户端断开连接
    void onNewConnection();  // 处理新连接

private:
    QMap<QTcpSocket*, int> clientMap;  // 存储客户端与 ID 映射
    int nextUserId = 1;  // 递增的用户 ID
    QTcpSocket* socket;
    QTimer* reconnectTimer;
    int generateUniqueUserId();  // 生成唯一的用户 ID
};

#endif // SERVER_H
