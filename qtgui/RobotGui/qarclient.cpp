/**
 * Qt Wrapper for ArClient
 */

#include <cassert>
#include <iostream>

#include <QLineF>
#include "ArClient.h"
#include "qarclient.h"

QArClient::QArClient(QObject *parent) :
    QObject(parent), ArClient(), _disconnected(false)
{

}

QArClient::QArClient(QString &sessionName, QObject *parent):
    QObject(parent), ArClient(sessionName.toStdString()), _disconnected(false)
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
    bool result = ArClient::connect(host.toStdString().c_str(),
                             port,
                             username.toStdString().c_str(),
                             password.toStdString().c_str());
    if (result)
    {
        _disconnected = false;
    }

    return result;
}

void QArClient::stop()
{
    ArClient::stop();
}

void QArClient::serverShutdown()
{
    if (!_disconnected)
    {
        disconnected("Server shutdown.");
        _disconnected = true;
    }
}

void QArClient::connectionError()
{
    // FIXME: do nothing?
}

void QArClient::disconnect()
{
    ArClient::disconnect();
    QObject::disconnect();
}
