#include "mockserver.h"
#include <QDebug>

MockServer::MockServer(QObject *parent) : QTcpServer(parent) {
    connect(this, &QTcpServer::newConnection, this, &MockServer::onNewConnection);
}

void MockServer::onNewConnection() {
    QTcpSocket *clientSocket = nextPendingConnection();
    connect(clientSocket, &QTcpSocket::readyRead, this, &MockServer::onReadyRead);
}

void MockServer::onReadyRead() {
    QTcpSocket *clientSocket = static_cast<QTcpSocket*>(sender());
    QByteArray requestData = clientSocket->readAll();

    // 요청 처리 (에코 응답 예시)
    qDebug() << "Received from client:" << requestData;
    // 실제 게임 로직에 맞게 응답을 구성할 수 있음
    clientSocket->write(requestData);  // 클라이언트에 요청을 그대로 응답
}
