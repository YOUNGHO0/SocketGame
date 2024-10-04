#ifndef OMOKGAMECLIENT_H
#define OMOKGAMECLIENT_H

#include <QWidget>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QLabel>
#include <ui_OmokGameClient.h>
#include <QTcpSocket>

class OmokGameClient : public QWidget {
Q_OBJECT

public:
    explicit OmokGameClient(QWidget *parent = nullptr);
    ~OmokGameClient();

    void setPlayerStoneColor(bool isBlack);
    void drawBoard();
    void placeStone(const QPointF &position, int x, int y);
    bool isValidMove(int x, int y);
    void displayWinMessage(int winningStone);
    bool checkWinCondition(int x, int y);

signals:
    void stonePlaced(int x, int y);

public slots:
    void updateBoard(int x, int y, int stoneColor);

protected:
    void mousePressEvent(QMouseEvent *event) override;

private:
    Ui::OmokGameClient *ui;
    QGraphicsView *graphicsView;
    QGraphicsScene *scene;
    QLabel *playerInfoLabel;
    int board[15][15];
    bool isBlackTurn;
    bool playerIsBlack;
    QTcpSocket *socket;  // 서버와의 소켓 연결
    void sendMoveToServer(int x, int y);
};

#endif // OMOKGAMECLIENT_H
