#include "server.h"
#include <QDebug>  // 输出调试信息

// 构造函数
ChatServer::ChatServer(QObject *parent) : QTcpServer(parent) {}

// 启动服务器监听
void ChatServer::startServer(quint16 port) {
    if (this->listen(QHostAddress::Any, port)) {  // 监听所有 IP 地址
        qDebug() << "服务器启动，监听端口：" << port;
    } else {
        qDebug() << "服务器启动失败：" << this->errorString();
    }
}

// 处理新的客户端连接
void ChatServer::incomingConnection(qintptr socketDescriptor) {
    QTcpSocket *clientSocket = new QTcpSocket(this);  // 创建新客户端 socket
    clientSocket->setSocketDescriptor(socketDescriptor);  // 绑定 socket 描述符

    // 连接信号槽，监听客户端消息和断开事件
    connect(clientSocket, &QTcpSocket::readyRead, this, &ChatServer::onReadyRead);
    connect(clientSocket, &QTcpSocket::disconnected, this, &ChatServer::onClientDisconnected);

    clients[clientSocket] = "";  // 存储客户端
    qDebug() << "新客户端连接：" << socketDescriptor;
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
            client->write(data);
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
