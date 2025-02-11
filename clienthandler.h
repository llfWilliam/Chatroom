#ifndef CLIENTHANDLER_H
#define CLIENTHANDLER_H

#include <QThread>
#include <QTcpSocket>
#include <QMap>

class ClientHandler : public QObject {
    Q_OBJECT
public:
    ClientHandler(QTcpSocket *socket, int userId, QObject *parent = nullptr);
    ~ClientHandler();

public slots:
    void run();  // 在独立线程中运行
signals:
    void messageReceived(int userId, const QString &message);
    void clientDisconnected(int userId);
    void finished();  // 线程完成信号

private slots:
    void onReadyRead();
    void onClientDisconnected();
    void sendMessage(const QString &message);

private:
    QTcpSocket *clientSocket;
    int userId;
    void sendWelcomeMessage();
};

#endif
