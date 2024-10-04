#include <QApplication>
#include "omokgameclient.h"

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);

    // OmokGameClient 생성 및 실행
    OmokGameClient w;
    w.show();

    return a.exec();
}
