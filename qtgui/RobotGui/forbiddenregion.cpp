#include <iostream>
#include "forbiddenregion.h"

using namespace std;

ForbiddenRegion::ForbiddenRegion(qreal width, qreal height, QGraphicsItem *parent):
    MapItem(parent), _width(width), _height(height)
{
    // Set default colors
    setLineColor(QColor(Qt::gray));
    QColor fillColor(Qt::gray);
    fillColor.setAlpha(127);
    setFillColor(fillColor);
}

ForbiddenRegion::ForbiddenRegion(QGraphicsItem *parent)
{
    // Set default colors
    setLineColor(QColor(Qt::gray));
    QColor fillColor(Qt::gray);
    fillColor.setAlpha(127);
    setFillColor(fillColor);

    // Dimensions based on green trash bin in ENB 2F
    _width=460;
    _height=500;
}

QRectF ForbiddenRegion::boundingRect() const
{
    return QRectF(-_width/2, -_height/2, _width, _height);
}

void ForbiddenRegion::paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
            QWidget *widget)
{
    painter->setPen(QPen(getLineColor()));
    painter->setBrush(QBrush(getFillColor()));
    painter->drawRect(boundingRect());
}
