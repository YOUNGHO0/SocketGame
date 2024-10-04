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

    // ��û ó�� (���� ���� ����)
    qDebug() << "Received from client:" << requestData;
    // ���� ���� ������ �°� ������ ������ �� ����
    clientSocket->write(requestData);  // Ŭ���̾�Ʈ�� ��û�� �״�� ����
}
