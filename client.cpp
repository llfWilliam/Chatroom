#include "client.h"
#include <QDebug>

// 构造函数
ChatClient::ChatClient(QObject *parent) : QObject(parent) {
    socket = new QTcpSocket(this);  // 创建 TCP 连接
    reconnectTimer = new QTimer(this);// 创建定时器

    // 监听服务器返回的数据
    connect(socket, &QTcpSocket::readyRead, this, &ChatClient::onReadyRead);
    connect(socket, &QTcpSocket::disconnected, this, &ChatClient::onDisconnected);
    connect(socket, &QTcpSocket::connected, this, &ChatClient::onConnected);
    connect(reconnectTimer, &QTimer::timeout, this, &ChatClient::onReconnect);
}

// 连接服务器
void ChatClient::connectToServer(const QString &host, int port) {
    qDebug() << "尝试连接到服务器：" << host << ":" << port;
    socket->connectToHost(QHostAddress(host), port);
    QTimer::singleShot(5000, this, [this]() {
        if (socket->state() == QTcpSocket::ConnectedState) {
            qDebug() << "连接成功（5秒检查）";
            emit connected();
            reconnectTimer->stop();
        } else {
            qDebug() << "连接失败（5秒检查）：" << socket->errorString();
            emit connectionFailed(socket->errorString());
            reconnectTimer->start(5000);  // 5秒后重试
        }
    });
}
void ChatClient::sendMessage(const QString &message) {
    if (socket->state() == QTcpSocket::ConnectedState) {
        socket->write(message.toUtf8());
        socket->flush();
    } else {
        qDebug() << "未连接到服务器";
    }
}

// 读取服务器返回的消息
void ChatClient::onReadyRead() {
    QByteArray data = socket->readAll();
    QString message = QString::fromUtf8(data);
    qDebug() << "收到消息：" << message;

    // 发射信号，将接收到的消息传递给 MainWindow
    emit messageReceived(message);
}
void ChatClient::onConnected() {
    qDebug() << "与服务器连接成功";
    reconnectTimer->stop();
}

void ChatClient::onDisconnected() {
    qDebug() << "与服务器断开连接";
    emit disconnected();
    reconnectTimer->start(5000);  // 断开连接后 5 秒后重试
}

void ChatClient::onReconnect() {
    qDebug() << "重新尝试连接服务器...";
    connectToServer("127.0.0.1", 12345);
}
