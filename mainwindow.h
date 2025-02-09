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

private:
    ChatClient *client;
    QTextEdit *chatBox;
    QLineEdit *messageInput;
    QPushButton *sendButton;
};
