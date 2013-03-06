#ifndef MAPITEM_H
#define MAPITEM_H

#include <QColor>
#include <QGraphicsItem>

class MapItem : public QGraphicsItem
{
public:
    explicit MapItem(QGraphicsItem *parent = 0);
    void setLineColor(QColor color);
    QColor getLineColor();
    void setFillColor(QColor color);
    QColor getFillColor();

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent * event);

private:
    QColor _lineColor;
    QColor _fillColor;
};

#endif // MAPITEM_H
