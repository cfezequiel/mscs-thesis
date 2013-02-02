/**
 * Qt Wrapper for ArClient
 */

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

