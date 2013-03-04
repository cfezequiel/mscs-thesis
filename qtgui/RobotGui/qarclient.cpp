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

void QArClient::updateNumbersReceived(ArRobotInfo robotInfo)
{
    emit updatePose(robotInfo);
}

void QArClient::updateStringsReceived(ArRobotInfo robotInfo)
{
    emit updatePose(robotInfo);
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
void QArClient::mapChanged(ArMap *map)
{
    sendMap(map);
}

bool QArClient::connect(QString host, int port, QString username, QString password)
{
    return ArClient::connect(host.toStdString().c_str(),
                             port,
                             username.toStdString().c_str(),
                             password.toStdString().c_str());
}

void QArClient::stop()
{
    ArClient::stop();
}
