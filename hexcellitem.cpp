#include "hexcellitem.h"
#include "boardscene.h"
#include <QBrush>
#include <QPen>
#include <QGraphicsSceneDragDropEvent>
#include <QGraphicsSceneMouseEvent>
#include <QMimeData>
#include <QDebug>
#include <QFont>

HexCellItem::HexCellItem(const Cell& cell, QGraphicsItem* parent)
    : QGraphicsPolygonItem(parent), cellData(cell)
{
    setAcceptDrops(true);

    QBrush brush;
    if (cell.value == '1') {
        brush = QBrush(QColor("#d0f0c0"));
    } else if (cell.value == '2') {
        brush = QBrush(QColor("#c0d0f0"));
    } else if (cell.value == '~') {
        brush = QBrush(QColor("#66ccff"));
    } else if (cell.value == '#') {
        brush = QBrush(QColor("#999999"));
    } else {
        brush = QBrush(QColor("#eeeeee"));
    }

    setBrush(brush);
    setPen(QPen(QColor(50, 50, 50), 2));
}

int HexCellItem::getRow() const { return cellData.row; }
int HexCellItem::getCol() const { return cellData.col; }
char HexCellItem::getValue() const { return cellData.value; }
QString HexCellItem::getType() const { return cellData.type; }
bool HexCellItem::hasAgent() const { return agentPresent; }

// void HexCellItem::setAgent(int playerId) {
//     agentPresent = true;
//     agentPlayer = playerId;

//     if (!agentIcon) {
//         agentIcon = new QGraphicsTextItem(this);
//     }

void HexCellItem::setAgent(int playerId) {
    agentPresent = true;
    agentPlayer = playerId;

    if (!agentIcon) {
        agentIcon = new QGraphicsTextItem(this);
    }

    QString text;
    if (playerId == 1) {
        text = "🤖";
    } else if (playerId == 2) {
        text = "👾";
    }

    agentIcon->setPlainText(text);
    agentIcon->setFont(QFont("Arial", 20));
    agentIcon->setPos(boundingRect().center() - QPointF(10, 20));

    qDebug() << "Agent" << playerId << "placed at cell (" << getRow() << "," << getCol() << ")";
}

void HexCellItem::clearAgent() {
    agentPresent = false;
    agentPlayer = 0;
    if (agentIcon) {
        delete agentIcon;
        agentIcon = nullptr;
    }
}

int HexCellItem::getAgentPlayer() const {
    return agentPlayer;
}

void HexCellItem::dragEnterEvent(QGraphicsSceneDragDropEvent* event) {
    if (event->mimeData()->hasText())
        event->acceptProposedAction();
}

void HexCellItem::dragMoveEvent(QGraphicsSceneDragDropEvent* event) {
    event->acceptProposedAction();
}


void HexCellItem::dropEvent(QGraphicsSceneDragDropEvent* event) {
    QString data = event->mimeData()->text();
    int playerId = data.toInt();

    auto* board = dynamic_cast<BoardScene*>(scene());
    if (!board || playerId != board->currentPlayer) {
        event->ignore();
        return;
    }

    if (hasAgent()) {
        event->ignore();
        return;
    }
    if (cellData.value == '~' || cellData.value == '#') {
        event->ignore();
        return;
    }
    if ((playerId == 1 && cellData.value != '1') ||
        (playerId == 2 && cellData.value != '2')) {
        event->ignore();
        return;
    }

    setAgent(playerId);

    board->currentPlayer = (board->currentPlayer == 1) ? 2 : 1;
    emit board->turnChanged(board->currentPlayer);

    event->acceptProposedAction();
}

void HexCellItem::mousePressEvent(QGraphicsSceneMouseEvent* event) {
    auto* board = dynamic_cast<BoardScene*>(scene());
    if (!board) return;

    if (event->button() == Qt::LeftButton) {
        if (hasAgent()) {
            board->highlightMoves(this);
        } else if (board->selectedAgent) {
            board->moveAgent(board->selectedAgent, this);
        }
    } else if (event->button() == Qt::RightButton) {
        qDebug() << "Right clicked on cell: (" << getRow() << "," << getCol()
        << "), value:" << getValue() << ", agent:" << hasAgent();
    }

    QGraphicsPolygonItem::mousePressEvent(event);
}

