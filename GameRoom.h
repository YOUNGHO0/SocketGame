#ifndef GAMEROOM_H
#define GAMEROOM_H

#include <QTcpSocket>
#include <QString>
#include <QList>
#include "ClientInfo.h" // ClientInfo ��� �߰�

class GameRoom {
public:
    GameRoom(const QString &gameType);
    void addClient(QTcpSocket *client, const QString &username);
    void removeClient(QTcpSocket *client);
    void sendMessageToClient(const QString &username, const QString &message);
    void sendMessageToAll(const QString &message); // ��� Ŭ���̾�Ʈ���� �޽��� ����
    void startGame();

private:
    QString m_gameType; // ���� ����
    QList<ClientInfo> m_clients; // �濡 �ִ� Ŭ���̾�Ʈ ���
};

#endif // GAMEROOM_H
