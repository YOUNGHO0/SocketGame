#include <QApplication>
#include "omokgameclient.h"

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);

    // OmokGameClient ���� �� ����
    OmokGameClient w;
    w.show();

    return a.exec();
}
