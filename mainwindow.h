#include <QMainWindow>
#include <QTextEdit>
#include <QLineEdit>
#include <QPushButton>
#include "client.h"

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = nullptr);

private slots:
    void onSendClicked();  // 发送按钮事件
    void onMessageReceived(const QString &message);
    void onConnected();
    void onDisconnected();
    void onConnectionFailed(const QString &error);

private:
    ChatClient *client;
    QTextEdit *chatBox;
    QLineEdit *messageInput;
    QPushButton *sendButton;
};
