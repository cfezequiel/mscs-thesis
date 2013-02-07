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
#include "obstacle.h"
#include "forbiddenregion.h"

using namespace std;

class MapScene : public QGraphicsScene
{
    Q_OBJECT

public:
    enum Mode { ModeView = 0, ModeAddObstacle };
    explicit MapScene(QObject *parent = 0);
    void renderMap(ArMap *map);
    void setMode(Mode mode);
    Mode mode() const;
    
protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent);

signals:
    void mapChanged(ArMap *map);
    
public slots:
    void updateRobotPose(ArRobotInfo *robotInfo);
    void updateRobotPath(Points *path);

private:
    void _modeAddObstacleRect(QPointF pos);
    Mode _mode;
    RobotObject *_robot;
    PathObject *_path;
    ArMap *_map;
};

#endif // MAPSCENE_H
