#include "mapobject.h"

MapObject::MapObject(QGraphicsItem *parent) :
    QGraphicsItem(parent)
{
    // Set parameters
    width = 501;
    height = 512;
    setLineColor(QColor(Qt::black));
    setFillColor(QColor(Qt::white));
}

void MapObject::setLineColor(QColor color)
{
    lineColor = color;
}

void MapObject::setFillColor(QColor color)
{
    fillColor = color;
}

void MapObject::setLineColor(Qt::GlobalColor color)
{
    lineColor = QColor(color);
}

void MapObject::setFillColor(Qt::GlobalColor color)
{
    fillColor = QColor(color);
}

QRectF MapObject::boundingRect() const
{
    // Assuming that (0,0) is the center of the object
    QRectF rectF(-width/2, -height/2, width, height);

    return rectF;
}

void MapObject::paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
           QWidget *widget)
{
    //FIXME: Draws only goals and robot homes
    // May need to add support later for forbidden areas

    // Paint a rectangle with a line indicating heading
    painter->setBrush(QBrush(fillColor));
    painter->drawRect(boundingRect());
    painter->setPen(QPen(lineColor));
    painter->drawLine(0, 0, width/2, 0);
}

