#ifndef OBSTACLE_H
#define OBSTACLE_H

#include <QAbstractGraphicsShapeItem>

class Obstacle : public QAbstractGraphicsShapeItem
{
public:
    Obstacle(QGraphicsItem* parent = 0);
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
               QWidget *widget);
};

#endif // OBSTACLE_H
