#include "omokgameclient.h"
#include "ui_OmokGameClient.h"
#include <QMouseEvent>
#include <QGraphicsEllipseItem>
#include <QMessageBox>
#include <QVBoxLayout>
#include <QBrush>
#include <QColor>
#include <QDataStream>  // 추가된 부분

OmokGameClient::OmokGameClient(QWidget *parent) :
        QWidget(parent), ui(new Ui::OmokGameClient), isBlackTurn(true), playerIsBlack(false) {
    ui->setupUi(this);

    // QGraphicsView와 QGraphicsScene 설정
    graphicsView = new QGraphicsView(this);
    scene = new QGraphicsScene(this);

    // 바둑판 배경색 설정 (연한 나무색)
    QBrush backgroundBrush(QColor(222, 184, 135));  // 나무색 (연한 갈색)
    scene->setBackgroundBrush(backgroundBrush);

    graphicsView->setScene(scene);
    graphicsView->setFixedSize(700, 700);  // 고정된 크기의 바둑판 설정
    scene->setSceneRect(0, 0, 600, 600);

    // 바둑판 상태 초기화 (0: 빈칸, 1: 흑돌, 2: 백돌)
    memset(board, 0, sizeof(board));

    // 바둑판 그리기
    drawBoard();

    // 흑돌/백돌 정보 표시하는 라벨 추가
    playerInfoLabel = new QLabel(this);
    playerInfoLabel->setText("Player's Stone: Waiting for server...");
    playerInfoLabel->setAlignment(Qt::AlignCenter);
    playerInfoLabel->setStyleSheet("font-size: 18px; color: blue;");
    playerInfoLabel->setFixedSize(200, 50);

    // 레이아웃 설정
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(graphicsView);
    layout->addWidget(playerInfoLabel);  // 플레이어 정보 라벨 추가
    setLayout(layout);

    // 서버와의 소켓 연결 초기화
    socket = new QTcpSocket(this);
    socket->connectToHost("localhost", 12345);  // 모킹 서버와 연결

    // 서버로부터 수신한 데이터를 처리할 슬롯 연결
    connect(socket, &QTcpSocket::readyRead, this, [this]() {
        QDataStream in(socket);
        int x, y, stoneColor;

        // 서버로부터 메시지 수신
        in >> x >> y >> stoneColor;
        updateBoard(x, y, stoneColor);  // 수신한 내용을 바둑판에 반영
    });
}

// ... 기존의 나머지 함수들 ...

void OmokGameClient::updateBoard(int x, int y, int stoneColor) {
    // 서버로부터 받은 정보를 바둑판에 반영
    QPointF position(x * 40 + 20, y * 40 + 20);
    placeStone(position, x, y);
}


///
OmokGameClient::~OmokGameClient() {
    delete ui;
}

void OmokGameClient::setPlayerStoneColor(bool isBlack) {
    playerIsBlack = isBlack;
    if (isBlack) {
        playerInfoLabel->setText("Player's Stone: Black");
    } else {
        playerInfoLabel->setText("Player's Stone: White");
    }
}

void OmokGameClient::drawBoard() {
    // 15x15 바둑판 그리기 (선)
    int gridSize = 40;  // 격자 하나의 크기
    for (int i = 0; i < 15; ++i) {
        // 가로선
        scene->addLine(20, i * gridSize + 20, 580, i * gridSize + 20);
        // 세로선
        scene->addLine(i * gridSize + 20, 20, i * gridSize + 20, 580);
    }
}

void OmokGameClient::mousePressEvent(QMouseEvent *event) {
    // 클릭한 좌표를 씬의 좌표로 변환
    QPointF clickPos = graphicsView->mapToScene(event->pos());

    // 바둑판의 격자 크기
    int gridSize = 40;

    // 클릭한 좌표를 격자 인덱스로 변환
    int x = static_cast<int>((clickPos.x() - 20) / gridSize);
    int y = static_cast<int>((clickPos.y() - 20) / gridSize);

    // 클릭한 위치가 유효한지 확인
    if (!isValidMove(x, y)) {
        return;
    }

    // 돌을 놓을 위치를 계산 (격자의 중앙 좌표)
    QPointF stonePos(x * gridSize + 20, y * gridSize + 20);

    // 돌을 놓음
    placeStone(stonePos, x, y);

    // 승리 조건 확인
    if (checkWinCondition(x, y)) {
        QMessageBox::information(this, "Victory", "You win!");
    }
}

void OmokGameClient::placeStone(const QPointF &position, int x, int y) {
    // 흑돌 또는 백돌 그리기
    int stoneSize = 30;  // 돌의 크기
    QGraphicsEllipseItem *stone = new QGraphicsEllipseItem(0, 0, stoneSize, stoneSize);
    stone->setPos(position.x() - stoneSize / 2, position.y() - stoneSize / 2);

    // 흑돌/백돌 색 설정
    if (isBlackTurn) {
        stone->setBrush(Qt::black);
        board[x][y] = 1;  // 바둑판 상태에 흑돌 저장
    } else {
        stone->setBrush(Qt::white);
        board[x][y] = 2;  // 바둑판 상태에 백돌 저장
    }

    isBlackTurn = !isBlackTurn;  // 턴 교체

    // 씬에 돌 추가
    scene->addItem(stone);
}

bool OmokGameClient::isValidMove(int x, int y) {
    // 돌을 놓을 수 없는 위치일 경우
    if (x < 0 || x >= 15 || y < 0 || y >= 15 || board[x][y] != 0) {
        return false;
    }

    return true;
}

void OmokGameClient::displayWinMessage(int winningStone) {
    QString message;
    if (winningStone == 1) {
        message = "Black wins!";
    } else if (winningStone == 2) {
        message = "White wins!";
    }
    QMessageBox::information(this, "Game Over", message);
}

bool OmokGameClient::checkWinCondition(int x, int y) {
    int currentStone = board[x][y];

    const int directions[4][2] = {
            {1, 0},  // 가로
            {0, 1},  // 세로
            {1, 1},  // 대각선
            {1, -1}  // 대각선
    };

    for (const auto& direction : directions) {
        int count = 1;

        // 한 방향으로 탐색
        for (int step = 1; step < 5; ++step) {
            int newX = x + direction[0] * step;
            int newY = y + direction[1] * step;

            if (newX < 0 || newX >= 15 || newY < 0 || newY >= 15) break;
            if (board[newX][newY] == currentStone) {
                count++;
            } else {
                break;
            }
        }

        // 반대 방향으로 탐색
        for (int step = 1; step < 5; ++step) {
            int newX = x - direction[0] * step;
            int newY = y - direction[1] * step;

            if (newX < 0 || newX >= 15 || newY < 0 || newY >= 15) break;
            if (board[newX][newY] == currentStone) {
                count++;
            } else {
                break;
            }
        }

        if (count >= 5) {
            displayWinMessage(currentStone);  // 승리 메시지 표시
            return true;
        }
    }

    return false;
}


