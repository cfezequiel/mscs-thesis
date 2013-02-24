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

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event);

private:
    qreal _width;
    qreal _height;
    QColor _lineColor;
    QColor _fillColor;
};

#endif // FORBIDDENREGION_H
