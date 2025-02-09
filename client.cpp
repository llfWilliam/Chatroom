#include "client.h"
#include <QDebug>

// 构造函数
ChatClient::ChatClient(QObject *parent) : QObject(parent) {
    socket = new QTcpSocket(this);  // 创建 TCP 连接

    // 监听服务器返回的数据
    connect(socket, &QTcpSocket::readyRead, this, &ChatClient::onReadyRead);
    connect(socket, &QTcpSocket::disconnected, this, &ChatClient::onDisconnected);
}

// 连接服务器
void ChatClient::connectToServer(const QString &host, quint16 port) {
    socket->connectToHost(host, port);
}

// 发送消息
void ChatClient::sendMessage(const QString &message) {
    if (socket->state() == QTcpSocket::ConnectedState) {
        socket->write(message.toUtf8());  // 发送消息
    }
}

// 读取服务器返回的消息
void ChatClient::onReadyRead() {
    QByteArray data = socket->readAll();
    QString message = QString::fromUtf8(data);
    qDebug() << "收到消息：" << message;
}

// 处理服务器断开
void ChatClient::onDisconnected() {
    qDebug() << "服务器已断开连接";
}
