#ifndef OBSTACLE_H
#define OBSTACLE_H

//FIXME: this is no longer used. Deprecate?

#include <vector>
#include <list>
#include <QAbstractGraphicsShapeItem>
using namespace std;

class Obstacle : public QAbstractGraphicsShapeItem
{
public:
    Obstacle(qreal width, qreal height, QGraphicsItem* parent = 0);
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
               QWidget *widget);
    void setWidth(qreal width) { _width = width; }
    void setHeight(qreal height) { _height = height; }
    list<QLineF> *sceneLines() const;

private:
    qreal _width;
    qreal _height;
};

#endif // OBSTACLE_H
