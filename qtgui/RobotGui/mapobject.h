#ifndef MAPOBJECT_H
#define MAPOBJECT_H

#include <QGraphicsItem>
#include <QPainter>
#include <QRectF>
#include <QColor>

class QPen;

class MapObject : public QGraphicsItem
{
public:
    explicit MapObject(QGraphicsItem *parent = 0);
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
               QWidget *widget);

    void setLineColor(QColor color);
    void setFillColor(QColor color);
    void setLineColor(Qt::GlobalColor color);
    void setFillColor(Qt::GlobalColor color);


protected:
    qreal width;
    qreal height;
    qreal th;
    QColor lineColor;
    QColor fillColor;

};

#endif // MAPOBJECT_H
