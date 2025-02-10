#include <QCoreApplication>
#include "server.h"

int main(int argc, char *argv[]) {
    QCoreApplication a(argc, argv);
    int port = 12345;
    ChatServer server;
    server.startServer(port);  // 确保 Server 类有 startServer() 方法
    return a.exec();
}
