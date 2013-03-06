#ifndef FORBIDDENREGION_H
#define FORBIDDENREGION_H

#include <QPen>
#include <QBrush>
#include <QPainter>

#include "mapitem.h"

class ForbiddenRegion : public MapItem
{
public:
    ForbiddenRegion(qreal width, qreal height, QGraphicsItem *parent = 0);
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
               QWidget *widget);

private:
    qreal _width;
    qreal _height;
};

#endif // FORBIDDENREGION_H
