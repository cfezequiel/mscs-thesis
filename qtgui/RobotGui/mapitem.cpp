#include <iostream>

#include "mapitem.h"

using namespace std;

MapItem::MapItem(QGraphicsItem *parent) :
    QGraphicsItem(parent)
{
    // do nothing
}

void MapItem::mousePressEvent(QGraphicsSceneMouseEvent * event)
{
    cout << "ForbiddenRegion::mousePressEvent" << endl;
}


