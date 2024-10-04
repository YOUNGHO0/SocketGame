#ifndef GAMEROOM_H
#define GAMEROOM_H

#include <QTcpSocket>
#include <QString>
#include <QList>
#include "ClientInfo.h" // ClientInfo 헤더 추가

class GameRoom {
public:
    GameRoom(const QString &gameType);
    void addClient(QTcpSocket *client, const QString &username);
    void removeClient(QTcpSocket *client);
    void sendMessageToClient(const QString &username, const QString &message);
    void sendMessageToAll(const QString &message); // 모든 클라이언트에게 메시지 전송
    void startGame();

private:
    QString m_gameType; // 게임 종류
    QList<ClientInfo> m_clients; // 방에 있는 클라이언트 목록
};

#endif // GAMEROOM_H
