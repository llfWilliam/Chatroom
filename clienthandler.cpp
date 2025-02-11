#include "clienthandler.h"
#include <QDebug>

ClientHandler::ClientHandler(QTcpSocket *socket, int userId, QObject *parent)
    : QObject(parent), clientSocket(socket), userId(userId)
{
    // 确保 socket 对象在不同线程中使用时不出问题
    if (clientSocket->thread() != QThread::currentThread()) {
        clientSocket->moveToThread(QThread::currentThread());
    }

    // 将信号与槽连接起来
}

ClientHandler::~ClientHandler() {
    // 确保 clientSocket 被正确删除
    if (clientSocket) {
        clientSocket->deleteLater();
    }
}

void ClientHandler::run() {
    // 发送欢迎消息
    QString welcomeMessage = QString("欢迎[User%1] 进入房间，快和他打个招呼吧！").arg(userId);
    emit messageReceived(userId, welcomeMessage);

    // 等待消息接收的操作
    connect(clientSocket, &QTcpSocket::readyRead, this, [this]() {
        QString data = clientSocket->readAll();
        QString message = data.section('|', 1, 1);
        emit messageReceived(userId, message);  // 将消息发送到外部处理
    });

    connect(clientSocket, &QTcpSocket::disconnected, this, [this]() {
        emit clientDisconnected(userId);
        emit finished();
    });
}


void ClientHandler::onReadyRead() {
    QByteArray data = clientSocket->readAll();
    QString message = QString::fromUtf8(data).trimmed();

    if (!message.isEmpty()) {
        emit messageReceived(userId, message);  // 通过信号传递消息
    }
}

void ClientHandler::onClientDisconnected() {
    emit clientDisconnected(userId);
    // 删除客户端 socket 并结束线程
    if (clientSocket) {
        clientSocket->deleteLater();
    }
    emit finished();  // 确保线程退出
}


void ClientHandler::sendMessage(const QString &message) {
    clientSocket->write(message.toUtf8());
    clientSocket->flush();
}
