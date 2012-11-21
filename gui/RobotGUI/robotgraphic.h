#ifndef ROBOTGRAPHIC_H
#define ROBOTGRAPHIC_H

#include <QGraphicsItem>

class RobotGraphic : public QGraphicsItem
{
public:
    RobotGraphic();
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
               QWidget *widget);
signals:
    
public slots:

private:
    // Bounding rectangle
    qreal _width;
    qreal _height;
    qreal _x;
    qreal _y;
    
};

#endif // ROBOTGRAPHIC_H
