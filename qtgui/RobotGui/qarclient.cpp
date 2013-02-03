/**
 * Qt Wrapper for ArClient
 */

#include <iostream>

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

void QArClient::getPathReceived(list<Point> points)
{
    // Correct for change in coordinate system (i.e. y = -y)
    for (list<Point>::iterator i = points.begin(); i != points.end(); i++)
    {
        i->y = -i->y;
    }
    emit updatePath(points);

    cout << "QArClient::getPathReceived" << endl;
}

