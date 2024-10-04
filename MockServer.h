#ifndef MOCKSERVER_H
#define MOCKSERVER_H

#include <QTcpServer>
#include <QTcpSocket>

class MockServer : public QTcpServer {
Q_OBJECT

public:
    explicit MockServer(QObject *parent = nullptr);

private slots:
    void onNewConnection();
    void onReadyRead();
};

#endif // MOCKSERVER_H
