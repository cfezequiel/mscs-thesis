#ifndef ZONE_H
#define ZONE_H

#include <QPainter>
#include <QRectF>
#include <QColor>

#include "mapitem.h"

class QPen;

class Zone : public MapItem
{
public:
    explicit Zone(QGraphicsItem *parent = 0);
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

#endif // ZONE_H
