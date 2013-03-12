#ifndef MAPSCENE_H
#define MAPSCENE_H

#include <list>

#include <QMainWindow>
#include <QGraphicsScene>
#include <QRectF>
#include <QPoint>
#include <QCursor>
#include <QKeyEvent>
#include <QTimer>

#include "Aria.h"
#include "robotobject.h"
#include "zone.h"
#include "mapdata.h"
#include "pathobject.h"
#include "forbiddenregion.h"

using namespace std;

class MapScene : public QGraphicsScene
{
    Q_OBJECT

public:
    enum Mode { ModeView = 0, ModeAddObstacle, ModeDelete };
    explicit MapScene(QObject *parent = 0);
    void renderMap(ArMap *map);
    void setMode(Mode mode);
    QList<QString> goalList();
    Mode mode() const;
    ForbiddenRegion *getMappedObstacle();
    ArMap * getMap();
    bool hasMap();
    
signals:
    void mapChanged(ArMap *map);
    void sendData(ArRobotInfo pose, QPointF obstaclePos);
    void stop();
    void untoggle();
    
public slots:
    void updateRobotPose(ArRobotInfo robotInfo);
    void updateRobotPath(Points *path);
    void updateMap();
    void clear();

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent);
    void keyPressEvent(QKeyEvent *keyEvent);
    void wheelEvent(QGraphicsSceneWheelEvent* wheelEvent);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent * mouseEvent);

private:
    void _modeAddObstacleRect(QPointF pos);
    QGraphicsView * viewFirst();
    Mode _mode;
    RobotObject *_robot;
    PathObject *_path;
    ArMap *_map;
    QList<QString> _goalNames;
    ForbiddenRegion *_mappedObstacle;
    QTimer _timer;
    QList<ForbiddenRegion *> _newObstacles;
};

#endif // MAPSCENE_H
