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
    // TODO: how to get the parent scene?

}


