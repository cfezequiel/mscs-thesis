#ifndef MAPSCENE_H
#define MAPSCENE_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QRectF>
#include <QPoint>
#include <QCursor>

#include "Aria.h"
#include "ArNetworking.h"
#include "qarclient.h"
#include "robotobject.h"
#include "mapobject.h"

class MapScene : public QGraphicsScene
{
    Q_OBJECT

public:
    explicit MapScene(QObject *parent = 0);
    void renderMap(ArMap *map);
    
protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent);

signals:
    
public slots:
    void updateRobotPose(ArRobotInfo * robotInfo);

private:
    RobotObject *_robot;
};

#endif // MAPSCENE_H
