#include "robotobject.h"

RobotObject::RobotObject(QGraphicsItem *parent) :
    QGraphicsItem(parent)
{
    height = 393;
    width = 237;
    lineColor = QColor(Qt::black);
    fillColor = QColor(Qt::red);
}

QRectF RobotObject::boundingRect() const
{
    // Assuming that (0,0) is the center of the object
    return QRectF(-width/2, -height/2, width, height);
}

void RobotObject::paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
           QWidget *widget)
{
    painter->setBrush(QBrush(fillColor));
    painter->drawEllipse(boundingRect());
    painter->setPen(QPen(lineColor));
    painter->drawLine(0,0, 0, -height/2);
}

void RobotObject::advance(int phase)
{
    //FIXME: do nothing?
}
