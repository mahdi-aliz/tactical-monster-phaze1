#ifndef HEXCELLITEM_H
#define HEXCELLITEM_H

#include <QGraphicsPolygonItem>
#include <QGraphicsTextItem>
#include "boardparser.h"
#include <QFont>

class HexCellItem : public QGraphicsPolygonItem
{
public:
    HexCellItem(const Cell& cell, QGraphicsItem* parent = nullptr);

    int getRow() const;
    int getCol() const;
    char getValue() const;
    QString getType() const;

    bool hasAgent() const;
    void setAgent(int playerId); // بازیکن 1 یا 2
    void clearAgent();
    int getAgentPlayer() const;


protected:
    void dragEnterEvent(QGraphicsSceneDragDropEvent* event) override;
    void dragMoveEvent(QGraphicsSceneDragDropEvent* event) override;
    void dropEvent(QGraphicsSceneDragDropEvent* event) override;
    void mousePressEvent(QGraphicsSceneMouseEvent* event) override;

private:
    Cell cellData;
    bool agentPresent = false;
    int agentPlayer = 0;
    QGraphicsTextItem* agentIcon = nullptr;
};

#endif // HEXCELLITEM_H
