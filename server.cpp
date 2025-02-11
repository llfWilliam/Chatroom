#include "server.h"
#include "clienthandler.h"
#include <QDebug>  // 输出调试信息

// 构造函数
ChatServer::ChatServer(QObject *parent)
    : QTcpServer(parent)
{
}

void ChatServer::startServer(quint16 port) {
    if (this->listen(QHostAddress::Any, port)) {
        qDebug() << "服务器启动，监听端口：" << port;
    } else {
        qDebug() << "服务器启动失败：" << this->errorString();
    }
    connect(this, &QTcpServer::newConnection, this, &ChatServer::onNewConnection);
}
int ChatServer::generateUniqueUserId() {
    return nextUserId++;
}

// 处理新的客户端连接
void ChatServer::onNewConnection() {
    QTcpSocket *clientSocket = this->nextPendingConnection();
    // 当客户端成功连接时，触发 connected 信号
    int newUserId = generateUniqueUserId();
    clientMap[clientSocket] = newUserId;
    ClientHandler *clientHandler = new ClientHandler(clientSocket, newUserId);
    QThread *thread = new QThread(this);
    clientHandler->moveToThread(thread);
    clientSocket->write(QString("ID_ASSIGNED|%1|").arg(newUserId).toUtf8());
    clientSocket->flush();
    connect(thread, &QThread::started, clientHandler, &ClientHandler::run);
    connect(clientHandler, &ClientHandler::messageReceived, this, &ChatServer::onClientMessageReceived);
    connect(clientHandler, &ClientHandler::clientDisconnected, this, &ChatServer::onClientDisconnected);
    connect(clientHandler, &ClientHandler::finished, thread, &QThread::quit);
    thread->start();

}

void ChatServer::onClientMessageReceived(int userId, const QString &message) {
    // 广播消息给所有客户端（不包括发送者）
    for (auto it = clientMap.begin(); it != clientMap.end(); ++it) {
        QTcpSocket *otherClient = it.key();
        if (clientMap[otherClient] != userId) {
            QString formattedMessage = QString("[User%1]: %2").arg(userId).arg(message);
            otherClient->write(formattedMessage.toUtf8());  // 发送格式化后的消息
            otherClient->flush();
        }
    }
}


void ChatServer::onClientDisconnected(int userId) {
    // 从客户端映射中移除该用户
    for (auto it = clientMap.begin(); it != clientMap.end(); ++it) {
        if (it.value() == userId) {
            clientMap.remove(it.key());
            break;
        }
    }
    qDebug() << "用户 ID " << userId << " 断开连接";
}

