#ifndef QARCLIENT_H
#define QARCLIENT_H

#include <list>
#include <QObject>
#include <QMetaType>
#include "ArClient.h"
#include "mapdata.h"
#include "forbiddenregion.h"

class QArClient : public QObject, public ArClient
{
    Q_OBJECT
public:
    explicit QArClient(QObject *parent = 0);
    bool connect(QString host, int port, QString username, QString password);

signals:
    void updatePose(ArRobotInfo robotInfo);
    void updateNumbers(ArRobotInfo robotInfo);
    void updateStrings(ArRobotInfo robotInfo);
    void updatePath(Points *path);

protected:
    void updateNumbersReceived(ArRobotInfo robotInfo);
    void updateStringsReceived(ArRobotInfo robotInfo);
    void getPathReceived(Points *path);

public slots:
    void mapChanged(ArMap *map);

};

#endif // QARCLIENT_H
