#ifndef CLIENTINFO_H
#define CLIENTINFO_H

#include <QTcpSocket>
#include <QString>

struct ClientInfo {
    QTcpSocket *socket;  // Ŭ���̾�Ʈ ����
    QString username;     // Ŭ���̾�Ʈ �̸� �Ǵ� ID

    ClientInfo(QTcpSocket *s, const QString &name) : socket(s), username(name) {}
};

#endif // CLIENTINFO_H
