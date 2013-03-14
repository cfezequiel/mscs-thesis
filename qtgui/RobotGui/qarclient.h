#ifndef QARCLIENT_H
#define QARCLIENT_H

#include <list>
#include <QObject>
#include <QMetaType>
#include <QString>
#include "ArClient.h"
#include "mapdata.h"
#include "forbiddenregion.h"

class QArClient : public QObject, public ArClient
{
    Q_OBJECT

public:
    enum DisconnectReason {ServerShutdown=0, ConnectionError};
    explicit QArClient(QObject *parent = 0);
    QArClient(QString &sessionName, QObject *parent = 0);
    bool connect(QString host, int port, QString username, QString password);
    void disconnect();

signals:
    void updatePose(ArRobotInfo robotInfo);
    void updateNumbers(ArRobotInfo robotInfo);
    void updateStrings(ArRobotInfo robotInfo);
    void updatePath(Points *path);
    void disconnected(QString reason);

protected:
    void updateNumbersReceived(ArRobotInfo robotInfo);
    void updateStringsReceived(ArRobotInfo robotInfo);
    void getPathReceived(Points *path);
    void serverShutdown();
    void connectionError();

public slots:
    void mapChanged(ArMap *map);
    void stop();
    void ratioDrive(double transRatio, double rotRatio, double scale);

private:
    bool _disconnected;

};

#endif // QARCLIENT_H
