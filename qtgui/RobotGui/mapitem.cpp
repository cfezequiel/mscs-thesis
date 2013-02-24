#include "mapitem.h"

MapItem::MapItem(QGraphicsItem *parent) :
    QGraphicsItem(parent)
{
    // do nothing
}

void MapItem::mousePressEvent(QGraphicsSceneMouseEvent * event)
{
    cout << "ForbiddenRegion::mousePressEvent" << endl;

    // if mapscene's state is 'delete', then delete this object from scene
    //TODO
}


