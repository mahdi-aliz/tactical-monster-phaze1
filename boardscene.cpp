#include "boardscene.h"
#include "hexcellitem.h"
#include <QDebug>
#include <cmath>

BoardScene::BoardScene(QObject *parent)
    : QGraphicsScene(parent), currentPlayer(1)
{
    setSceneRect(0, 0, 1200, 1000);
}

QPointF BoardScene::computeHexPosition(int row, int col)
{
    double dx = 0.75 * hexSize;
    double dy = std::sqrt(0.18) * hexSize;
    double x = row * dx;
    double y = col * dy;

    return QPointF(x, y);
}

QPolygonF BoardScene::createHexagon(QPointF center)
{
    QPolygonF hex;
    for (int i = 0; i < 6; ++i) {
        double angle = M_PI / 3.0 * i;
        hex << QPointF(center.x() + hexSize * std::cos(angle),
                       center.y() + hexSize * std::sin(angle));
    }
    return hex;
}

void BoardScene::buildHexGridWithRows(const std::vector<Cell>& fiveRows, const std::vector<Cell>& fourRows)
{
    clear();
    cellItems.clear();

    int colHeights[] = {5, 4, 5, 4, 5, 4, 5, 4, 5};
    int cellIndexEven = 0;
    int cellIndexOdd = 0;

    for (int col = 0; col < 9; ++col) {
        int height = colHeights[col];
        for (int row = 0; row < height; ++row) {
            int logicalRow = (col % 2 == 0) ? row * 2 : row * 2 + 1;

            Cell cell;
            if (logicalRow % 2 == 0) {
                if (cellIndexEven < fiveRows.size())
                    cell = fiveRows[cellIndexEven++];
                else continue;
            } else {
                if (cellIndexOdd < fourRows.size())
                    cell = fourRows[cellIndexOdd++];
                else continue;
            }

            cell.row = logicalRow;
            cell.col = col;

            QPointF center = computeHexPosition(cell.row, cell.col);
            QPolygonF hex = createHexagon(center);

            HexCellItem* item = new HexCellItem(cell);
            item->setPolygon(hex);
            item->setPos(center);

            addItem(item);
            cellItems.push_back(item);
        }
    }
}

void BoardScene::moveAgent(HexCellItem* from, HexCellItem* to)
{
    if (!from || !to) return;
    if (!from->hasAgent()) return;
    if (to->hasAgent()) return;
    if (to->getValue() == '#' || to->getValue() == '~') return;
    if (from->getAgentPlayer() != currentPlayer) return;

    int agentPlayer = from->getAgentPlayer();
    from->clearAgent();
    to->setAgent(agentPlayer);

    clearHighlights();
    selectedAgent = nullptr;

    currentPlayer = (currentPlayer == 1) ? 2 : 1;
    emit turnChanged(currentPlayer);

}

void BoardScene::highlightMoves(HexCellItem* from)
{
    if (!from || from->getAgentPlayer() != currentPlayer) return;
    clearHighlights();
    selectedAgent = from;

    int baseRow = from->getRow();
    int baseCol = from->getCol();

    for (auto* cell : cellItems) {
        int dr = std::abs(cell->getRow() - baseRow);
        int dc = std::abs(cell->getCol() - baseCol);

        if ((dr == 1 && dc == 0) || (dr == 0 && dc == 1) || (dr == 1 && dc == 1)) {
            if (cell->getValue() != '#' && cell->getValue() != '~' && !cell->hasAgent()) {
                cell->setBrush(QBrush(QColor("#ffffaa")));
            }
        }
    }
}

void BoardScene::clearHighlights()
{
    for (auto* cell : cellItems) {
        if (cell->hasAgent()) {
            cell->setAgent(cell->getAgentPlayer());
        } else {
            if (cell->getValue() == '1') cell->setBrush(QBrush(QColor("#d0f0c0")));
            else if (cell->getValue() == '2') cell->setBrush(QBrush(QColor("#c0d0f0")));
            else if (cell->getValue() == '~') cell->setBrush(QBrush(QColor("#66ccff")));
            else if (cell->getValue() == '#') cell->setBrush(QBrush(QColor("#999999")));
            else cell->setBrush(QBrush(QColor("#eeeeee")));
        }
    }
}
