/**
 * Qt Wrapper for ArClient
 */

#include <cassert>
#include <iostream>

#include <QLineF>
#include "ArClient.h"
#include "qarclient.h"

QArClient::QArClient(QObject *parent) :
    QObject(parent), ArClient()
{

}

void QArClient::updateNumbersReceived(ArRobotInfo *robotInfo)
{
    emit updateNumbers(robotInfo);
}

void QArClient::updateStringsReceived(ArRobotInfo *robotInfo)
{
    emit updateStrings(robotInfo);
}

void QArClient::getPathReceived(Points *path)
{
    // Correct for change in coordinate system (i.e. y = -y)
    for (list<Point>::iterator i = path->data.begin(); i != path->data.end(); i++)
    {
        i->y = -i->y;
    }
    emit updatePath(path);
}

// Slots
void QArClient::addObstacle(ForbiddenRegion *fr)
{
    ArMap *map = getMap();
    assert(map != NULL);

    // Get points
    QRectF rect = fr->boundingRect();
    QPointF center = fr->scenePos();
    qreal rotation = fr->rotation();
    ArPose pose(0, 0, rotation);
    ArPose fromPose(center.x() - rect.width()/2, -(center.y() - rect.height()/2), 0);
    ArPose toPose(center.x() + rect.width()/2, -(center.y() + rect.height()/2), 0);

    // Add obstacle to map
    ArMapObject *frObject = new ArMapObject("ForbiddenArea", pose, "", "ICON", "", true, fromPose, toPose);
    list<ArMapObject *> *mapObjects = map->getMapObjects();
    mapObjects->push_back(frObject);
    map->setMapObjects(mapObjects);

    // Send map updates
    stop();
    //ArUtil::sleep(100);
    sendMap(map);
    ArUtil::sleep(100);
    resume();
}

