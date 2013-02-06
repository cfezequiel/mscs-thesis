#include <QPen>
#include <QPainter>
#include "obstacle.h"

Obstacle::Obstacle(qreal width, qreal height, QGraphicsItem *parent):
    QAbstractGraphicsShapeItem(parent),
    _width(width), _height(height)
{
}

QRectF Obstacle::boundingRect() const
{
    return QRectF(-_width/2, -_height/2, _width, _height);
}

void Obstacle::paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
           QWidget *widget)
{
    //FIXME: for now, just paint a rectangle
    painter->setPen(QPen(Qt::blue));
    painter->drawRect(boundingRect());
}

list<QLineF> *Obstacle::sceneLines() const
{
    list<QLineF> *lines_ = new list<QLineF>;
    QRectF rect = boundingRect();

    qreal halfWidth = _width / 2;
    qreal halfHeight = _height / 2;
    QPointF center = scenePos();
    QPointF topLeft = QPointF(center.x() - halfWidth, center.y() + halfHeight);
    QPointF topRight = QPointF(center.x() + halfWidth, center.y() + halfHeight);
    QPointF bottomRight = QPointF(center.x() + halfWidth, center.y() - halfHeight);
    QPointF bottomLeft = QPointF(center.x() - halfWidth, center.y() - halfHeight);

    lines_->push_back(QLineF(topLeft, topRight));
    lines_->push_back(QLineF(topRight, bottomRight));
    lines_->push_back(QLineF(bottomRight, bottomLeft));
    lines_->push_back(QLineF(bottomLeft, topLeft));

    return lines_;
}
