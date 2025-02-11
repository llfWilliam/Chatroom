#include "client.h"
#include <QDebug>

ChatClient::ChatClient(QObject *parent) : QObject(parent) {
    socket = new QTcpSocket(this);  // 创建 TCP 连接
    reconnectTimer = new QTimer(this);  // 创建定时器

    // 监听服务器返回的数据
    connect(socket, &QTcpSocket::readyRead, this, &ChatClient::onReadyRead);
    connect(socket, &QTcpSocket::disconnected, this, &ChatClient::onDisconnected);
    connect(socket, &QTcpSocket::connected, this, &ChatClient::onConnected);
    connect(socket, &QTcpSocket::errorOccurred, this, &ChatClient::onErrorOccurred);
    connect(reconnectTimer, &QTimer::timeout, this, &ChatClient::onReconnect);
}

void ChatClient::connectToServer(const QString &host, int port) {
    qDebug() << "尝试连接到服务器：" << host << ":" << port;
    socket->connectToHost(QHostAddress(host), port);
}

void ChatClient::sendMessage(const QString &message) {
    if (socket->state() == QTcpSocket::ConnectedState && userId != -1) {
        // 将用户 ID 和消息内容一起发送
        QString messageWithId = QString::number(userId) + "|" + message;
        socket->write(messageWithId.toUtf8());
        socket->flush();
        // 直接显示自己发的消息
        emit messageReceived("[我]：" + message);
    } else {
        qDebug() << "未连接到服务器或未分配 ID";
        emit errorOccurred("未连接到服务器或未分配 ID");
    }
}

void ChatClient::onReadyRead() {
    QByteArray data = socket->readAll();
    QString message = QString::fromUtf8(data).trimmed();  // 去除首尾空格，避免解析错误
    if (userId == -1 && message.startsWith("ID_ASSIGNED|")) {  // 只在未分配 ID 时处理
        // 服务器返回的 ID
        userId = message.section('|', 1, 1).toInt();
        emit messageReceived(QString("[你的ID: %1]").arg(userId));  // 显示自己分配的 ID
    } else {
        // 处理普通消息
        emit messageReceived(message);  // 显示其他的聊天消息
    }
}

void ChatClient::onConnected() {
    qDebug() << "与服务器连接成功";
    emit connected();  // 立即通知连接成功
    reconnectTimer->stop();
}

void ChatClient::onErrorOccurred(QAbstractSocket::SocketError socketError) {
    QString errorMsg;
    switch (socketError) {
    case QAbstractSocket::HostNotFoundError:
        errorMsg = "错误: 服务器地址无法解析";
        break;
    case QAbstractSocket::ConnectionRefusedError:
        errorMsg = "错误: 服务器拒绝连接";
        break;
    case QAbstractSocket::RemoteHostClosedError:
        errorMsg = "错误: 远程主机关闭连接";
        break;
    default:
        errorMsg = "网络错误: " + socket->errorString();
    }
    qDebug() << errorMsg;
    emit errorOccurred(errorMsg);
    emit connectionFailed(errorMsg);  // 立即通知失败，不再手动延迟
    if (socket->state() != QTcpSocket::ConnectedState) {
        qDebug() << "错误发生，5 秒后尝试重连...";
        reconnectTimer->start(5000);
    }
}

void ChatClient::onDisconnected() {
    qDebug() << "与服务器断开连接";
    emit disconnected();
    reconnectTimer->start(5000);  // 断开连接后 5 秒后重试
}

void ChatClient::onReconnect() {
    qDebug() << "重新尝试连接服务器...";
    int userId=-1;
    connectToServer("127.0.0.1", 12345);
}

void ChatClient::disconnectFromServer() {
    if (socket->state() == QTcpSocket::ConnectedState) {
        socket->disconnectFromHost();
    }
}
