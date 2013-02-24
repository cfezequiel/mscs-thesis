#include <iostream>
#include "forbiddenregion.h"

using namespace std;

ForbiddenRegion::ForbiddenRegion(qreal width, qreal height, QGraphicsItem *parent):
    MapItem(parent), _width(width), _height(height)
{
    _lineColor = QColor(Qt::gray);
    _fillColor = QColor(Qt::gray);
}

QRectF ForbiddenRegion::boundingRect() const
{
    return QRectF(-_width/2, -_height/2, _width, _height);
}

void ForbiddenRegion::paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
            QWidget *widget)
{
    painter->setPen(QPen(_lineColor));
    painter->setBrush(QBrush(_fillColor));
    painter->drawRect(boundingRect());
}
