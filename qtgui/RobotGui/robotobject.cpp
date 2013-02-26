#include "robotobject.h"

RobotObject::RobotObject(QGraphicsItem *parent) :
    QGraphicsItem(parent)
{
    _width = 393;
    _length = 445;
    lineColor = QColor(Qt::black);
    fillColor = QColor(Qt::red);
}

QRectF RobotObject::boundingRect() const
{
    // Assuming that (0,0) is the center of the object
    return QRectF(-_width/2, -_length/2, _width, _length);
}

void RobotObject::paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
           QWidget *widget)
{
    painter->setBrush(QBrush(fillColor));
    painter->drawEllipse(boundingRect());
    painter->setPen(QPen(lineColor));
    painter->drawLine(0,0, 0, -_length/2);
}

qreal RobotObject::width() const
{
    return _width;
}

qreal RobotObject::length() const
{
    return _length;
}

void RobotObject::setPose(ArRobotInfo *pose)
{
    _pose = pose;
}

ArRobotInfo* RobotObject::getPose()
{
    return _pose;
}
