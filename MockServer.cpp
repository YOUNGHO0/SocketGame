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

    // ��û �����͸� ���ڿ��� ��ȯ
    QString request = QString::fromUtf8(requestData);

    // Ŭ���̾�Ʈ ��û ó��
    QStringList parts = request.split(":");
    if (parts.size() >= 2 && parts[0] == "chat") {
        // �޽��� ���� ��û: "chat:����ڸ�:�޽�������"
        QString username = parts[1];
        QString message = parts.mid(2).join(":"); // ����ڸ��� ������ �޽��� ����

        // �ش� ������� �޽����� ���� ���� ��� Ŭ���̾�Ʈ���� ����
        for (auto room : gameRooms) {
            room->sendMessageToAll(username + ": " + message); // ��� ����ڿ��� �޽��� ����
        }
    } else {
        // �Ϲ� ��û ó�� (���� �� ����, Ŭ���̾�Ʈ �߰� ��)
        handleClientRequest(clientSocket, request);
    }
}

void MockServer::handleClientRequest(QTcpSocket *clientSocket, const QString &request) {
    QStringList parts = request.split(":"); // ��û ���� ����: "create_room:��������" �Ǵ� "add_client:����ڸ�"

    if (parts.size() == 2 && parts[0] == "create_room") {
        createGameRoom(parts[1], clientSocket);
    } else if (parts.size() == 2 && parts[0] == "add_client") {
        // Ŭ���̾�Ʈ�� �濡 �߰��� �� ������� �̸��� �Բ� �߰�
        for (auto room : gameRooms) {
            room->addClient(clientSocket, parts[1]); // ������� �̸��� ���� �߰�
        }
    }
}

void MockServer::createGameRoom(const QString &gameType, QTcpSocket *clientSocket) {
    // �� ���� �� Ŭ���̾�Ʈ �߰�
    GameRoom *newRoom = new GameRoom(gameType);
    newRoom->addClient(clientSocket, "Anonymous"); // �⺻ ����� �̸����� �߰� (����� �̸��� ���߿� ���� ����)

    QString roomName = gameType + QString::number(gameRooms.size()); // �� �̸� ����
    gameRooms.insert(roomName, newRoom);

    // �� ���� �Ϸ� �޽��� ����
    QString message = "Game room created: " + roomName;
    newRoom->sendMessageToAll(message);
}

void MockServer::onClientDisconnected() {
    QTcpSocket *clientSocket = static_cast<QTcpSocket*>(sender());
    connectedClients.removeAll(clientSocket);

    // �� �濡�� Ŭ���̾�Ʈ ���� ���� �߰� �ʿ�
    for (auto room : gameRooms) {
        room->removeClient(clientSocket);
    }

    clientSocket->deleteLater();
}
