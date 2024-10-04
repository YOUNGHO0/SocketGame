#include "omokgameclient.h"
#include "ui_OmokGameClient.h"
#include <QMouseEvent>
#include <QGraphicsEllipseItem>
#include <QMessageBox>
#include <QVBoxLayout>
#include <QBrush>
#include <QColor>
#include <QDataStream>  // �߰��� �κ�

OmokGameClient::OmokGameClient(QWidget *parent) :
        QWidget(parent), ui(new Ui::OmokGameClient), isBlackTurn(true), playerIsBlack(false) {
    ui->setupUi(this);

    // QGraphicsView�� QGraphicsScene ����
    graphicsView = new QGraphicsView(this);
    scene = new QGraphicsScene(this);

    // �ٵ��� ���� ���� (���� ������)
    QBrush backgroundBrush(QColor(222, 184, 135));  // ������ (���� ����)
    scene->setBackgroundBrush(backgroundBrush);

    graphicsView->setScene(scene);
    graphicsView->setFixedSize(700, 700);  // ������ ũ���� �ٵ��� ����
    scene->setSceneRect(0, 0, 600, 600);

    // �ٵ��� ���� �ʱ�ȭ (0: ��ĭ, 1: �浹, 2: �鵹)
    memset(board, 0, sizeof(board));

    // �ٵ��� �׸���
    drawBoard();

    // �浹/�鵹 ���� ǥ���ϴ� �� �߰�
    playerInfoLabel = new QLabel(this);
    playerInfoLabel->setText("Player's Stone: Waiting for server...");
    playerInfoLabel->setAlignment(Qt::AlignCenter);
    playerInfoLabel->setStyleSheet("font-size: 18px; color: blue;");
    playerInfoLabel->setFixedSize(200, 50);

    // ���̾ƿ� ����
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(graphicsView);
    layout->addWidget(playerInfoLabel);  // �÷��̾� ���� �� �߰�
    setLayout(layout);

    // �������� ���� ���� �ʱ�ȭ
    socket = new QTcpSocket(this);
    socket->connectToHost("localhost", 12345);  // ��ŷ ������ ����

    // �����κ��� ������ �����͸� ó���� ���� ����
    connect(socket, &QTcpSocket::readyRead, this, [this]() {
        QDataStream in(socket);
        int x, y, stoneColor;

        // �����κ��� �޽��� ����
        in >> x >> y >> stoneColor;
        updateBoard(x, y, stoneColor);  // ������ ������ �ٵ��ǿ� �ݿ�
    });
}

// ... ������ ������ �Լ��� ...

void OmokGameClient::updateBoard(int x, int y, int stoneColor) {
    // �����κ��� ���� ������ �ٵ��ǿ� �ݿ�
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
    // 15x15 �ٵ��� �׸��� (��)
    int gridSize = 40;  // ���� �ϳ��� ũ��
    for (int i = 0; i < 15; ++i) {
        // ���μ�
        scene->addLine(20, i * gridSize + 20, 580, i * gridSize + 20);
        // ���μ�
        scene->addLine(i * gridSize + 20, 20, i * gridSize + 20, 580);
    }
}

void OmokGameClient::mousePressEvent(QMouseEvent *event) {
    // Ŭ���� ��ǥ�� ���� ��ǥ�� ��ȯ
    QPointF clickPos = graphicsView->mapToScene(event->pos());

    // �ٵ����� ���� ũ��
    int gridSize = 40;

    // Ŭ���� ��ǥ�� ���� �ε����� ��ȯ
    int x = static_cast<int>((clickPos.x() - 20) / gridSize);
    int y = static_cast<int>((clickPos.y() - 20) / gridSize);

    // Ŭ���� ��ġ�� ��ȿ���� Ȯ��
    if (!isValidMove(x, y)) {
        return;
    }

    // ���� ���� ��ġ�� ��� (������ �߾� ��ǥ)
    QPointF stonePos(x * gridSize + 20, y * gridSize + 20);

    // ���� ����
    placeStone(stonePos, x, y);

    // �¸� ���� Ȯ��
    if (checkWinCondition(x, y)) {
        QMessageBox::information(this, "Victory", "You win!");
    }
}

void OmokGameClient::placeStone(const QPointF &position, int x, int y) {
    // �浹 �Ǵ� �鵹 �׸���
    int stoneSize = 30;  // ���� ũ��
    QGraphicsEllipseItem *stone = new QGraphicsEllipseItem(0, 0, stoneSize, stoneSize);
    stone->setPos(position.x() - stoneSize / 2, position.y() - stoneSize / 2);

    // �浹/�鵹 �� ����
    if (isBlackTurn) {
        stone->setBrush(Qt::black);
        board[x][y] = 1;  // �ٵ��� ���¿� �浹 ����
    } else {
        stone->setBrush(Qt::white);
        board[x][y] = 2;  // �ٵ��� ���¿� �鵹 ����
    }

    isBlackTurn = !isBlackTurn;  // �� ��ü

    // ���� �� �߰�
    scene->addItem(stone);
}

bool OmokGameClient::isValidMove(int x, int y) {
    // ���� ���� �� ���� ��ġ�� ���
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
            {1, 0},  // ����
            {0, 1},  // ����
            {1, 1},  // �밢��
            {1, -1}  // �밢��
    };

    for (const auto& direction : directions) {
        int count = 1;

        // �� �������� Ž��
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

        // �ݴ� �������� Ž��
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
            displayWinMessage(currentStone);  // �¸� �޽��� ǥ��
            return true;
        }
    }

    return false;
}


