#include <iostream>
#include <QCoreApplication>
#include "MockServer.h"
int main(int argc, char *argv[]){

    QCoreApplication a(argc, argv);

    // Mock Server 시작
    MockServer server;
    if (!server.listen(QHostAddress::Any, 12345)) {  // 원하는 포트 번호
        qFatal("Unable to start the server: %s.", qPrintable(server.errorString()));
    }
    qDebug() << "Mock server started on port" << server.serverPort();

    // 서버가 항상 떠 있도록 이벤트 루프 실행
    return a.exec(); // 메인 이벤트 루프를 계속 실행

}