#ifndef FORBIDDENREGION_H
#define FORBIDDENREGION_H

#include <QPen>
#include <QBrush>
#include <QPainter>

#include "mapobject.h"

class ForbiddenRegion : public QGraphicsItem
{
public:
    ForbiddenRegion(qreal width, qreal height, QGraphicsItem *parent = 0);
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
               QWidget *widget);

private:
    qreal _width;
    qreal _height;
    QColor _lineColor;
    QColor _fillColor;
};

#endif // FORBIDDENREGION_H
