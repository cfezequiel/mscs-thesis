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
void QArClient::mapChanged(ArMap *map)
{
    //stop();
    sendMap(map);
    //ArUtil::sleep(100);
    resume(); //FIXME: resume hardcoded as 'gotoGoal' operation
}

bool QArClient::connect(QString host, int port, QString username, QString password)
{
    return ArClient::connect(host.toStdString().c_str(),
                             port,
                             username.toStdString().c_str(),
                             password.toStdString().c_str());
}

