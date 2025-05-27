#ifndef BOARDSCENE_H
#define BOARDSCENE_H

#include <QGraphicsScene>
#include <vector>
#include "boardparser.h"
#include "hexcellitem.h"

class BoardScene : public QGraphicsScene
{
    Q_OBJECT
public:
    explicit BoardScene(QObject *parent = nullptr);

    void buildHexGrid(const std::vector<Cell>& cells);
    QPointF computeHexPosition(int col, int row);
    QPolygonF createHexagon(QPointF center);
    void buildHexGridWithRows(const std::vector<Cell>& fiveRows, const std::vector<Cell>& fourRows);

    void moveAgent(HexCellItem* from, HexCellItem* to);
    void highlightMoves(HexCellItem* from);
    void clearHighlights();

    HexCellItem* selectedAgent = nullptr;
    int currentPlayer ;
signals:
    void turnChanged(int currentPlayer);

private:
    std::vector<HexCellItem*> cellItems;
    double hexSize = 70.0;
};

#endif // BOARDSCENE_H
