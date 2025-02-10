#ifndef SERVER_H
#define SERVER_H
#include <QTimer>      //为定时做准备
#include <QTcpServer>  // TCP 服务器模块
#include <QTcpSocket>  // TCP 通信模块
#include <QMap>        // 用于存储多个客户端连接
#include <QObject>     // Qt 对象基类

class ChatServer : public QTcpServer {
    Q_OBJECT
public:
    explicit ChatServer(QObject *parent = nullptr);
        // 构造函数
    void startServer(quint16 port);  // 启动服务器

protected:
    void incomingConnection();  // 处理新连接

private slots:
    void onReadyRead();       // 读取客户端消息
    void onClientDisconnected();  // 处理客户端断开
    void onReconnect();

private:
    QMap<QTcpSocket*, QString> clients;  // 存储客户端连接信息
    QTcpSocket* socket;
    QTimer* reconnectTimer;
};

#endif // SERVER_H
