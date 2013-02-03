#include <climits>
#include <cmath>
#include <QPainter>
#include <QPen>
#include "pathobject.h"

PathObject::PathObject(QGraphicsItem *parent):
    QGraphicsItem(parent)
{
    _lineColor = QColor(Qt::blue);
}

QRectF PathObject::boundingRect() const
{
    if (_points.empty())
    {
        return QRectF(0,0,0,0);
    }

    int top = INT_MIN;
    int left = INT_MAX;
    int bottom = INT_MAX;
    int right = INT_MIN;

    // Find the min/max x and y points
    for (list<Point>::const_iterator i = _points.begin(); i != _points.end(); i++)
    {
        if (i->x <= left)
        {
            left = i->x;
        }
        if (i->x >= right)
        {
            right = i->x;
        }
        if (i->y >= top)
        {
            top = i->y;
        }
        if (i->y <= bottom)
        {
            bottom = i->y;
        }
    }

    // Get the width and height (add some padding)
    qreal width = abs(right - left) + 1;
    qreal height = abs(top - bottom) + 1;

    return QRectF(-width/2, -height/2, width, height);
}

void PathObject::paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
       QWidget *widget)
{
    // Set pen
    painter->setPen(QPen(_lineColor));

    // Paint a line based on the points
    qreal x1,x2,y1,y2;
    for (list<Point>::iterator i = _points.begin(); i != _points.end(); i++)
    {
        if (i == _points.begin())
        {
            x1 = i->x;
            y1 = i->y;
            continue;
        }
        else
        {
            x2 = i->x;
            y2 = i->y;
        }

        // Paint line
        painter->drawLine(x1, y1, x2, y2);

        // Store current value as prev value
        x1 = x2;
        y1 = y2;
    }
}

void PathObject::update(list<Point> points)
{
    _points = points;
    QGraphicsItem::update(boundingRect());
}
