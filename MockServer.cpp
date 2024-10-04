#include "MockServer.h"
#include <QDebug>
#include <QDataStream>

MockServer::MockServer(QObject *parent) : QTcpServer(parent) {
    connect(this, &QTcpServer::newConnection, this, &MockServer::onNewConnection);
}

void MockServer::onNewConnection() {
    QTcpSocket *clientSocket = nextPendingConnection();
    connectedClients.append(clientSocket);

    connect(clientSocket, &QTcpSocket::readyRead, this, &MockServer::onReadyRead);
    connect(clientSocket, &QTcpSocket::disconnected, this, &MockServer::onClientDisconnected);
}

void MockServer::onReadyRead() {
    QTcpSocket *clientSocket = static_cast<QTcpSocket*>(sender());
    QByteArray requestData = clientSocket->readAll();

    // 요청 데이터를 문자열로 변환
    QString request = QString::fromUtf8(requestData);

    // 클라이언트 요청 처리
    QStringList parts = request.split(":");
    if (parts.size() >= 2 && parts[0] == "chat") {
        // 메시지 전송 요청: "chat:사용자명:메시지내용"
        QString username = parts[1];
        QString message = parts.mid(2).join(":"); // 사용자명을 제외한 메시지 내용

        // 해당 사용자의 메시지를 같은 방의 모든 클라이언트에게 전송
        for (auto room : gameRooms) {
            room->sendMessageToAll(username + ": " + message); // 모든 사용자에게 메시지 전송
        }
    } else {
        // 일반 요청 처리 (게임 방 생성, 클라이언트 추가 등)
        handleClientRequest(clientSocket, request);
    }
}

void MockServer::handleClientRequest(QTcpSocket *clientSocket, const QString &request) {
    QStringList parts = request.split(":"); // 요청 포맷 예시: "create_room:게임종류" 또는 "add_client:사용자명"

    if (parts.size() == 2 && parts[0] == "create_room") {
        createGameRoom(parts[1], clientSocket);
    } else if (parts.size() == 2 && parts[0] == "add_client") {
        // 클라이언트를 방에 추가할 때 사용자의 이름을 함께 추가
        for (auto room : gameRooms) {
            room->addClient(clientSocket, parts[1]); // 사용자의 이름과 소켓 추가
        }
    }
}

void MockServer::createGameRoom(const QString &gameType, QTcpSocket *clientSocket) {
    // 방 생성 및 클라이언트 추가
    GameRoom *newRoom = new GameRoom(gameType);
    newRoom->addClient(clientSocket, "Anonymous"); // 기본 사용자 이름으로 추가 (사용자 이름을 나중에 설정 가능)

    QString roomName = gameType + QString::number(gameRooms.size()); // 방 이름 생성
    gameRooms.insert(roomName, newRoom);

    // 방 생성 완료 메시지 전송
    QString message = "Game room created: " + roomName;
    newRoom->sendMessageToAll(message);
}

void MockServer::onClientDisconnected() {
    QTcpSocket *clientSocket = static_cast<QTcpSocket*>(sender());
    connectedClients.removeAll(clientSocket);

    // 각 방에서 클라이언트 제거 로직 추가 필요
    for (auto room : gameRooms) {
        room->removeClient(clientSocket);
    }

    clientSocket->deleteLater();
}
