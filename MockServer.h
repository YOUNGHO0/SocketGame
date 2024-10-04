#ifndef MOCKSERVER_H
#define MOCKSERVER_H

#include <QTcpServer>
#include <QTcpSocket>
#include <QList>
#include <QMap>
#include "GameRoom.h" // GameRoom 헤더 추가

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

    QList<QTcpSocket*> connectedClients; // 연결된 클라이언트 소켓 리스트
    QMap<QString, GameRoom*> gameRooms; // 방 목록 관리
};

#endif // MOCKSERVER_H
