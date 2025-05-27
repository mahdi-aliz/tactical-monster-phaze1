#include "view.h"
#include "boardparser.h"
#include "boardscene.h"
#include "agenticon.h"

#include <QGraphicsView>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QInputDialog>
#include <QMessageBox>
#include <QLabel>

View::View(QWidget *parent) : QWidget(parent) {
    promptPlayerNames();
    setupUI();
    loadBoard();
}

void View::promptPlayerNames() {
    bool ok1, ok2;
    player1Name = QInputDialog::getText(this, "Player 1", "Enter name for Player 1:", QLineEdit::Normal, "", &ok1);
    player2Name = QInputDialog::getText(this, "Player 2", "Enter name for Player 2:", QLineEdit::Normal, "", &ok2);

    if (!ok1 || player1Name.isEmpty()) player1Name = "Player 1";
    if (!ok2 || player2Name.isEmpty()) player2Name = "Player 2";
}

void View::setupUI() {
    auto *mainLayout = new QHBoxLayout(this);


    turnLabel = new QLabel(QString("%1's turn").arg(player1Name), this);
    turnLabel->setAlignment(Qt::AlignCenter);
    turnLabel->setStyleSheet("font-size: 18px; font-weight: bold;");

    auto *topLayout = new QVBoxLayout;
    topLayout->addWidget(turnLabel);

    auto *agentMenu1 = new QVBoxLayout;
    auto *agentMenu2 = new QVBoxLayout;

    for (int i = 0; i < 3; ++i) {
        agentMenu1->addWidget(new AgentIcon("1", this));
        agentMenu2->addWidget(new AgentIcon("2", this));
    }

    auto *leftPanel = new QWidget;
    leftPanel->setLayout(agentMenu1);

    auto *rightPanel = new QWidget;
    rightPanel->setLayout(agentMenu2);

    boardContainer = new QWidget(this);
    boardLayout = new QVBoxLayout(boardContainer);

    mainLayout->addWidget(leftPanel);
    mainLayout->addSpacing(50);
    mainLayout->addWidget(boardContainer, 1);
    mainLayout->addSpacing(50);
    mainLayout->addWidget(rightPanel);

    topLayout->addLayout(mainLayout);
    setLayout(topLayout);
}
void View::onTurnChanged(int currentPlayer) {
    QString name = (currentPlayer == 1) ? player1Name : player2Name;
    turnLabel->setText(QString("%1's turn").arg(name));
}

void View::loadBoard() {
    auto *board = new BoardScene(this);

    connect(board, &BoardScene::turnChanged, this, &View::onTurnChanged);



    BoardParser parser;
    ParsedMap parsed = parser.parse(":/grids/grids/grid7.txt");

    board->buildHexGridWithRows(parsed.fiveCellRows, parsed.fourCellRows);

    auto *view = new QGraphicsView(this);
    view->setScene(board);
    view->setAcceptDrops(true);
    view->setRenderHint(QPainter::Antialiasing);
    view->setAlignment(Qt::AlignCenter);
    view->setMinimumSize(1000, 800);
    view->setSceneRect(board->itemsBoundingRect());
    view->scale(1, 0.9);

    boardLayout->addWidget(view);
}

















