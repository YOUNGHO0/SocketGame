#include <QApplication>
#include "omokgameclient.h"
#include "MockServer.h"

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);

    // Mock Server ����
    MockServer server;
    if (!server.listen(QHostAddress::Any, 12345)) {  // ���ϴ� ��Ʈ ��ȣ
        qFatal("Unable to start the server: %s.", qPrintable(server.errorString()));
    }
    qDebug() << "Mock server started on port" << server.serverPort();

    // OmokGameClient ���� �� ����
    OmokGameClient w;
    w.show();

    return a.exec();
}
