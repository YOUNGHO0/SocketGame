#ifndef CLIENTINFO_H
#define CLIENTINFO_H

#include <QTcpSocket>
#include <QString>

struct ClientInfo {
    QTcpSocket *socket;  // 클라이언트 소켓
    QString username;     // 클라이언트 이름 또는 ID

    ClientInfo(QTcpSocket *s, const QString &name) : socket(s), username(name) {}
};

#endif // CLIENTINFO_H
