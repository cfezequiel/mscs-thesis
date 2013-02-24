#ifndef MAPITEM_H
#define MAPITEM_H

#include <QGraphicsItem>

class MapItem : public QGraphicsItem
{
public:
    explicit MapItem(QGraphicsItem *parent = 0);

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent * event);

signals:
    void deleteItem(QGraphicsItem *item);
};

#endif // MAPITEM_H
