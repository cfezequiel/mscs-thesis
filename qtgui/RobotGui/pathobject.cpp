#include <iostream>
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
    _boundingRect = new QRectF(-0.5, -0.5, 1, 1);
    _center = new QPointF(0,0);
}

PathObject::~PathObject()
{
    delete _boundingRect;
    delete _center;
}

QRectF PathObject::boundingRect() const
{
    assert(_boundingRect != NULL);

    return *_boundingRect;
}

void PathObject::paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
       QWidget *widget)
{
    // Set pen
    painter->setPen(QPen(QBrush(_lineColor), 50));

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
#if 0 // This is not the right center
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
#else
    // This is the right center
    return *_center;
#endif
}

void PathObject::setPath(Points *path)
{
    assert(path != NULL);

    // Schedule redraw of existing path
    //update(boundingRect());

    // Update path
    _points = *path;

    // Update bounding rect
    updateBoundingRect();

    // Tell the graphics scene that item's geometry is going to be updated
    prepareGeometryChange();
}

void PathObject::updateBoundingRect()
{
    assert(_boundingRect != NULL);

    int top = INT_MAX;
    int left = INT_MAX;
    int bottom = INT_MIN;
    int right = INT_MIN;

    // Find the min/max x and y points
    for (list<Point>::const_iterator i = _points.data.begin();
         i != _points.data.end(); i++)
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

    // Find center based on bounding 'points'
    _center->setX(left + abs(left - right)/2);
    _center->setY(bottom - abs(bottom - top)/2);

    // Get the width and height (add some padding)
    qreal pad = 1000;
    qreal width = abs(right - left) + pad;
    qreal height = abs(top - bottom) + pad;

    _boundingRect->setRect(-width/2, -height/2, width, height);
}
