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
void QArClient::addObstacle(Obstacle *obs)
{
    ArMap *map = getMap();
    assert(map != NULL);

    // Edit the map with 'obs'
    list<QLineF> *lines = obs->sceneLines();

    // Add each line to the map
    vector<ArLineSegment> *mapLines = map->getLines();
    for (list<QLineF>::iterator i = lines->begin(); i != lines->end(); i++)
    {
        // Don't forget to negate y due to change in coordinate system
        ArLineSegment line(i->x1(), -i->y1(), i->x2(), -i->y2());
        mapLines->push_back(line);
    }
    map->setLines(mapLines);

    // Send map updates
    sendMap(map);
}

