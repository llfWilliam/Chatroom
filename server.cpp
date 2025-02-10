#include "server.h"
#include <QDebug>  // 输出调试信息

// 构造函数
ChatServer::ChatServer(QObject *parent)
    : QTcpServer(parent), reconnectTimer(new QTimer(this))  // 初始化成员
{
    // 连接定时器的超时信号
    connect(reconnectTimer, &QTimer::timeout, this, &ChatServer::onReconnect);
}


// 启动服务器监听
void ChatServer::startServer(quint16 port) {
    if (this->listen(QHostAddress::Any, port)) {  // 监听所有 IP 地址
        qDebug() << "服务器启动，监听端口：" << port;
    } else {
        qDebug() << "服务器启动失败：" << this->errorString();
    }
    connect(this, &QTcpServer::newConnection, this, &ChatServer::incomingConnection);  // 新连接
}

// 处理新的客户端连接
void ChatServer::incomingConnection() {
    // 使用 nextPendingConnection() 获取新连接的套接字
    QTcpSocket *clientSocket = this->nextPendingConnection();

    if (clientSocket) {
        // 连接信号槽，监听客户端消息和断开事件
        connect(clientSocket, &QTcpSocket::readyRead, this, &ChatServer::onReadyRead);
        connect(clientSocket, &QTcpSocket::disconnected, this, &ChatServer::onClientDisconnected);

        clients[clientSocket] = "";  // 存储客户端
        qDebug() << "新客户端连接：" << clientSocket->peerAddress().toString();
    }
}


// 读取客户端发送的消息
void ChatServer::onReadyRead() {
    QTcpSocket *clientSocket = qobject_cast<QTcpSocket*>(sender());  // 获取触发事件的客户端
    if (!clientSocket) return;

    QByteArray data = clientSocket->readAll();  // 读取数据
    QString message = QString::fromUtf8(data);  // 转换为字符串

    qDebug() << "收到消息：" << message;

    // 群发消息给所有在线用户
    for (auto it = clients.begin(); it != clients.end(); ++it) {
        QTcpSocket *client = it.key();  // 直接取 key，而不是 keys()
        if (client != clientSocket) {
            client->write(data);  // 将消息发送给其他客户端
        }
    }
}

// 处理客户端断开
void ChatServer::onClientDisconnected() {
    QTcpSocket *clientSocket = qobject_cast<QTcpSocket*>(sender());
    if (!clientSocket) return;

    qDebug() << "客户端断开：" << clients[clientSocket];
    clients.remove(clientSocket);  // 从列表中移除
    clientSocket->deleteLater();  // 释放资源
}
void ChatServer::onReconnect() {
    qDebug() << "尝试重新连接...";
    // 实现重试逻辑（例如重新启动服务器或连接数据库等）
}
