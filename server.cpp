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
    connect(this, &QTcpServer::newConnection, this, &ChatServer::onNewConnection);  // 新连接
}
int ChatServer::generateUniqueUserId() {
    return nextUserId++;
}
// 处理新的客户端连接
void ChatServer::onNewConnection() {
    QTcpSocket *clientSocket = this->nextPendingConnection();
    int newUserId = generateUniqueUserId();
    clientMap[clientSocket] = newUserId;
    // 先发送 ID 给新用户
    clientSocket->write(QString("ID_ASSIGNED|" + QString::number(newUserId)).toUtf8());
    //生成欢迎消息
    QString welcomeMessage = QString("欢迎[User%1] 进入房间，快和他打个招呼吧！").arg(newUserId);
    //广播欢迎消息给新用户
    for (auto it = clientMap.begin(); it != clientMap.end(); ++it) {
        QTcpSocket *otherClient = it.key();
        otherClient->write(welcomeMessage.toUtf8());
    }

    connect(clientSocket, &QTcpSocket::readyRead, this, &ChatServer::onReadyRead);
    connect(clientSocket, &QTcpSocket::disconnected, this, &ChatServer::onClientDisconnected);
}


// 读取客户端发送的消息
void ChatServer::onReadyRead() {
    QTcpSocket *clientSocket = qobject_cast<QTcpSocket*>(sender());
    if (!clientSocket) return;

    QByteArray data = clientSocket->readAll();
    QString message = QString::fromUtf8(data);

    int senderId = clientMap.value(clientSocket, -1);
    if (senderId != -1) {
        QString formattedMessage = QString("[User%1]: %2").arg(senderId).arg(message.section('|', 1, 1));

        // 广播消息（但不发给自己）
        for (auto it = clientMap.begin(); it != clientMap.end(); ++it) {
            QTcpSocket *otherClient = it.key();  // 直接使用 key，而不是 keys()
            if (otherClient != clientSocket) {
                otherClient->write(formattedMessage.toUtf8());
            }
        }
    }
}

// 处理客户端断开
void ChatServer::onClientDisconnected() {
    QTcpSocket *clientSocket = qobject_cast<QTcpSocket*>(sender());
    if (!clientSocket) return;

    qDebug() << "客户端断开：" << clientMap.value(clientSocket, -1);
    clientMap.remove(clientSocket);
    clientSocket->deleteLater();
}

void ChatServer::onReconnect() {
    qDebug() << "尝试重新连接...";
    // 实现重试逻辑（例如重新启动服务器或连接数据库等）
}
