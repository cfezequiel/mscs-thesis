#ifndef MAPSCENE_H
#define MAPSCENE_H

#include <list>

#include <QMainWindow>
#include <QGraphicsScene>
#include <QRectF>
#include <QPoint>
#include <QCursor>

#include "Aria.h"
#include "ArNetworking.h"
#include "robotobject.h"
#include "mapobject.h"
#include "mapdata.h"
#include "pathobject.h"

using namespace std;

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
    void updateRobotPath(list<Point> points);

private:
    RobotObject *_robot;
    PathObject *_path;
};

#endif // MAPSCENE_H
