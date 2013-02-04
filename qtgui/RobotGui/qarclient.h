#ifndef QARCLIENT_H
#define QARCLIENT_H

#include <list>
#include <QObject>
#include <QMetaType>
#include "ArClient.h"
#include "mapdata.h"



class QArClient : public QObject, public ArClient
{
    Q_OBJECT
public:
    explicit QArClient(QObject *parent = 0);

signals:
    void updateNumbers(ArRobotInfo *robotInfo);
    void updateStrings(ArRobotInfo *robotInfo);
    void updatePath(Points *path);

protected:
    void updateNumbersReceived(ArRobotInfo *robotInfo);
    void updateStringsReceived(ArRobotInfo *robotInfo);
    void getPathReceived(Points *path);

public slots:

private:

};

#endif // QARCLIENT_H
