#ifndef MOCKSERVER_H
#define MOCKSERVER_H

#include <QTcpServer>
#include <QTcpSocket>
#include <QList>
#include <QMap>
#include "GameRoom.h" // GameRoom ��� �߰�

class MockServer : public QTcpServer {
Q_OBJECT

public:
    explicit MockServer(QObject *parent = nullptr);

private slots:
    void onNewConnection();
    void onReadyRead();
    void onClientDisconnected();

private:
    void handleClientRequest(QTcpSocket *clientSocket, const QString &request);
    void createGameRoom(const QString &gameType, QTcpSocket *clientSocket);

    QList<QTcpSocket*> connectedClients; // ����� Ŭ���̾�Ʈ ���� ����Ʈ
    QMap<QString, GameRoom*> gameRooms; // �� ��� ����
};

#endif // MOCKSERVER_H
