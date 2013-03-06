#include <iostream>

#include "mapitem.h"

using namespace std;

MapItem::MapItem(QGraphicsItem *parent) :
    QGraphicsItem(parent)
{
    // do nothing
}

void MapItem::mousePressEvent(QGraphicsSceneMouseEvent * event)
{
    // TODO: how to get the parent scene?

}

void MapItem::setLineColor(QColor color)
{
    _lineColor = color;
}

void MapItem::setFillColor(QColor color)
{
    _fillColor = color;
}

QColor MapItem::getLineColor()
{
    return _lineColor;
}

QColor MapItem::getFillColor()
{
    return _fillColor;
}
