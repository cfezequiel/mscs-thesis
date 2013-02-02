#ifndef ROBOTOBJECT_H
#define ROBOTOBJECT_H

#include <QGraphicsItem>
#include <QPainter>


class RobotObject : public QGraphicsItem
{
public:
    explicit RobotObject(QGraphicsItem *parent = 0);
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
               QWidget *widget);
    QRectF boundingRect() const;

protected:
    void advance(int phase);

    qreal width;
    qreal height;
    QColor lineColor;
    QColor fillColor;

private:
    qreal _th;
    qreal _x;
    qreal _y;


};

#endif // ROBOTOBJECT_H
