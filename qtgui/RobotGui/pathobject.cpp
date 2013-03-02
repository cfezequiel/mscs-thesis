#include <assert.h>
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
    if (_points.data.empty())
    {
        return QRectF(0,0,1,1);
    }

    int top = INT_MAX;
    int left = INT_MAX;
    int bottom = INT_MIN;
    int right = INT_MIN;

    // Find the min/max x and y points
    for (list<Point>::const_iterator i = _points.data.begin(); i != _points.data.end(); i++)
    {
        if (i->x <= left)
        {
            left = i->x;
        }
        if (i->x >= right)
        {
            right = i->x;
        }
        if (i->y <= top)
        {
            top = i->y;
        }
        if (i->y >= bottom)
        {
            bottom = i->y;
        }
    }

    // Get the width and height (add some padding)
    qreal pad = 50;
    qreal width = abs(right - left) + pad;
    qreal height = abs(top - bottom) + pad;

    return QRectF(-width/2 - pad, -height/2 - pad,
                  width + 2 * pad, height + 2 * pad);
}

void PathObject::paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
       QWidget *widget)
{
    // Set pen
    painter->setPen(QPen(_lineColor));

    // Paint a line based on the points
    qreal x1,x2,y1,y2;
    for (list<Point>::iterator i = _points.data.begin(); i != _points.data.end(); i++)
    {
        if (i == _points.data.begin())
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
        QPointF p1 = mapFromScene(QPointF(x1, y1));
        QPointF p2 = mapFromScene(QPointF(x2, y2));
        painter->drawLine(p1, p2);

        // Store current value as prev value
        x1 = x2;
        y1 = y2;
    }
}

QPointF PathObject::getPos()
{
    qreal sumX = 0;
    qreal sumY = 0;
    for (list<Point>::iterator i = _points.data.begin();
         i != _points.data.end(); i++)
    {
        sumX += i->x;
        sumY += i->y;
    }

    // Get centroid
    qreal n = _points.data.size();
    qreal cx = sumX / n;
    qreal cy = sumY / n;

    return QPointF(cx, cy);
}

void PathObject::setPath(Points *path)
{
    assert(path != NULL);
    _points = *path;
}
