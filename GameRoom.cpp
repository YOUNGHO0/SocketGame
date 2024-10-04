#include "GameRoom.h"
#include <QDataStream>

GameRoom::GameRoom(const QString &gameType) : m_gameType(gameType) {}

void GameRoom::addClient(QTcpSocket *client, const QString &username) {
    m_clients.append(ClientInfo(client, username)); // ClientInfo ����ü�� ���ϰ� ����ڸ� ����
}

void GameRoom::removeClient(QTcpSocket *client) {
    for (int i = 0; i < m_clients.size(); ++i) {
        if (m_clients[i].socket == client) {
            m_clients.removeAt(i); // Ŭ���̾�Ʈ ������ ��ġ�ϴ� ��� ����
            break; // Ŭ���̾�Ʈ�� ã���� ���� ����
        }
    }
}

void GameRoom::sendMessageToClient(const QString &username, const QString &message) {
    QByteArray messageData = message.toUtf8();
    for (const ClientInfo &clientInfo : m_clients) {
        if (clientInfo.username == username && clientInfo.socket->state() == QAbstractSocket::ConnectedState) {
            clientInfo.socket->write(messageData);
            clientInfo.socket->flush();
            break; // Ư�� Ŭ���̾�Ʈ���Ը� �޽����� �����Ƿ�, ������ �����մϴ�.
        }
    }
}

void GameRoom::sendMessageToAll(const QString &message) {
    QByteArray messageData = message.toUtf8();
    for (const ClientInfo &clientInfo : m_clients) {
        if (clientInfo.socket->state() == QAbstractSocket::ConnectedState) {
            clientInfo.socket->write(messageData);
            clientInfo.socket->flush();
        }
    }
}

void GameRoom::startGame() {
    sendMessageToAll("Game has started!");
}
