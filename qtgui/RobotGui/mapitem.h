#ifndef MAPITEM_H
#define MAPITEM_H

#include <QGraphicsItem>

class MapItem : public QGraphicsItem
{
public:
    explicit MapItem(QGraphicsItem *parent = 0);

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent * event);
};

#endif // MAPITEM_H
