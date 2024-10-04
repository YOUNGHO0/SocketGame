#include "GameRoom.h"
#include <QDataStream>

GameRoom::GameRoom(const QString &gameType) : m_gameType(gameType) {}

void GameRoom::addClient(QTcpSocket *client, const QString &username) {
    m_clients.append(ClientInfo(client, username)); // ClientInfo 구조체에 소켓과 사용자명 저장
}

void GameRoom::removeClient(QTcpSocket *client) {
    for (int i = 0; i < m_clients.size(); ++i) {
        if (m_clients[i].socket == client) {
            m_clients.removeAt(i); // 클라이언트 소켓이 일치하는 경우 제거
            break; // 클라이언트를 찾으면 루프 종료
        }
    }
}

void GameRoom::sendMessageToClient(const QString &username, const QString &message) {
    QByteArray messageData = message.toUtf8();
    for (const ClientInfo &clientInfo : m_clients) {
        if (clientInfo.username == username && clientInfo.socket->state() == QAbstractSocket::ConnectedState) {
            clientInfo.socket->write(messageData);
            clientInfo.socket->flush();
            break; // 특정 클라이언트에게만 메시지를 보내므로, 루프를 종료합니다.
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
