#include <QApplication>
#include "omokgameclient.h"
#include "MockServer.h"

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);

    // Mock Server 시작
    MockServer server;
    if (!server.listen(QHostAddress::Any, 12345)) {  // 원하는 포트 번호
        qFatal("Unable to start the server: %s.", qPrintable(server.errorString()));
    }
    qDebug() << "Mock server started on port" << server.serverPort();

    // OmokGameClient 생성 및 실행
    OmokGameClient w;
    w.show();

    return a.exec();
}
