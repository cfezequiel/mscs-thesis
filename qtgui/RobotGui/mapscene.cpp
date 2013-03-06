#include <cassert>
#include <cmath>
#include <cstdlib>
#include <iostream>

#include <sstream>
#include <vector>
#include <list>

#include <QCursor>
#include <QPoint>
#include <QSize>
#include <QPen>
#include <QGraphicsRectItem>
#include <QPointF>
#include <QGraphicsSceneHoverEvent>
#include <QGraphicsView>

#include "mapscene.h"

using namespace std;

MapScene::MapScene(QObject *parent) :
    QGraphicsScene(parent),
    _mode(ModeView),
    _mappedObstacle(NULL)
{
    // Do nothing
}

void MapScene::renderMap(ArMap *map)
{
    double x1,y1,x2,y2,th;
    double pad = 100.0;

    //NOTE: ARIA uses the "inverse-y" coordinate system
    // (y goes up as the value increases)
    // But Qt uses the regular image y-coordinate system

    // Set bounding rectangle
    ArPose minLinePose = map->getLineMinPose();
    ArPose maxLinePose = map->getLineMaxPose();
    minLinePose.getPose(&x1, &y1, &th); //th unused
    maxLinePose.getPose(&x2, &y2, &th); //th unused
    QRectF boundingRect(x1 - pad, - y2 -pad,
                        fabs(x2-x1) + 2*pad, fabs(y2-y1) + 2*pad);
    setSceneRect(boundingRect);

    // Render lines
    vector< ArLineSegment > *lines = map->getLines();
    for (vector< ArLineSegment >::iterator i = lines->begin();
         i != lines->end(); i++)
    {
        x1 = i->getX1();
        y1 = i->getY1();
        x2 = i->getX2();
        y2 = i->getY2();
        addLine(x1, -y1, x2, -y2);
    }

    // Render robot
    _robot = new RobotObject;
    _robot->setZValue(2);
    addItem(_robot);

    // Render map objects
    Zone *obj;
    string type;
    list< ArMapObject *> *objects = map->getMapObjects();
    for (list< ArMapObject *>::iterator i = objects->begin();
         i != objects->end(); i++)
    {
        type = (*i)->getType();
        ArPose pose = (*i)->getPose();
        th = pose.getTh();

        // If object is a region/line
        if ((*i)->hasFromTo())
        {
            ArPose fromPose, toPose;
            fromPose = (*i)->getFromPose(); // Top-left
            toPose = (*i)->getToPose(); // Bottom-right
            ArPose center = (*i)->findCenter();

            if (type == "ForbiddenArea")
            {
                qreal width = fabs(toPose.getX() - fromPose.getX());
                qreal height = fabs(toPose.getY() - fromPose.getY());

                ForbiddenRegion *fr = new ForbiddenRegion(width, height);
                fr->setPos(center.getX(), -center.getY());
                fr->setRotation(-th);
                addItem(fr);
            }
            else if (type == "ForbiddenLine")
            {
                // Ignore (make invisible)
            }
            else
            {
                cerr << "Unsupported MapObject region = " << type << endl;
            }
        }
        // If object is a point
        else
        {
            x1 = pose.getX();
            y1 = pose.getY();

            type = (*i)->getType();
            if (type == "GoalWithHeading")
            {
                obj = new Zone;

                // Set pos
                obj->setPos(x1,-y1);
                obj->setRotation(-th);
                obj->setLineColor(Qt::cyan);
                obj->setFillColor(Qt::darkCyan);
                addItem(obj);

                // Store goal name
                _goalNames.push_back(QString((*i)->getName()));
            }
            else if (type == "RobotHome")
            {
                obj = new Zone;

                // Set pose
                obj->setPos(x1,-y1);
                obj->setRotation(-th);
                obj->setLineColor(Qt::black);
                obj->setFillColor(QColor(0xbbeebb));
                addItem(obj);
            }
            else if (type == "Goal")
            {
                // This represents a mapped obstacle
                // Only get the first one found
                if (_mappedObstacle == NULL)
                {
                    qreal width = _robot->width();
                    qreal height = _robot->length();
                    _mappedObstacle = new ForbiddenRegion(width, height);
                    _mappedObstacle->setScale(2);
                    QColor color(Qt::black);
                    color.setAlpha(127);
                    _mappedObstacle->setFillColor(color);
                    _mappedObstacle->setPos(x1, -y1);
                }
            }
            else
            {
                cerr << "Unsupported MapObject point = " << type << endl;
            }

        }
    } // end render map objects

    // Store map
    _map = map;

    // Render path (should be invisible initially)
    _path = new PathObject;
    _path->setZValue(1);
    addItem(_path);
}

void MapScene::_modeAddObstacleRect(QPointF pos)
{
    // Set initial dimensions to robot dimensions
    qreal width = _robot->width();
    qreal length = _robot->length();

    // Add a forbidden region centered at 'pos'
    ForbiddenRegion *fr = new ForbiddenRegion(width, length);
    fr->setScale(2);
    addItem(fr);
    fr->setPos(pos);

    // Get poses
    QRectF rect = fr->boundingRect();
    QPointF center = fr->scenePos();
    qreal rotation = fr->rotation();
    ArPose pose(0, 0, rotation);
    ArPose fromPose(center.x() - rect.width()/2, -(center.y() - rect.height()/2), 0);
    ArPose toPose(center.x() + rect.width()/2, -(center.y() + rect.height()/2), 0);

    // Add obstacle to ArMap
    ArMapObject *frObject = new ArMapObject("ForbiddenArea", pose,
                                            "", "ICON", "", true, fromPose, toPose);
    list<ArMapObject *> *mapObjects = _map->getMapObjects();
    mapObjects->push_back(frObject);
    _map->setMapObjects(mapObjects);

    // Send signal that map was changed
    emit mapChanged(_map);

    // Log data
    sendData(_robot->getPose(), pos);
}

void MapScene::mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    QPointF pos = mouseEvent->scenePos();

    // Do something depending on the mode
    QGraphicsView *view = viewFirst();
    switch (_mode)
    {
    case ModeView:
        if (view->dragMode() != QGraphicsView::ScrollHandDrag)
        {
            view->setDragMode(QGraphicsView::ScrollHandDrag);
        }
        break;

    case ModeAddObstacle:
        if (mouseEvent->button() == Qt::LeftButton)
        {
            _modeAddObstacleRect(pos);
        }
        // Untoggle the add obstacle button
        untoggle();
        _mode = ModeView;
        break;

    default:
        // Do nothing
        break;
    }
}

void MapScene::keyPressEvent(QKeyEvent *keyEvent)
{
    // If spacebar pressed, stop the robot
    if (keyEvent->key() == Qt::Key_Space)
    {
        stop();
    }
}

// FIXME: this should connect to RobotObject not the scene
void MapScene::updateRobotPose(ArRobotInfo robotInfo)
{
    assert(_robot != NULL);

    // Redraw the robot on the map based on robot telemetry info
    qreal x = robotInfo.xpos;
    qreal y = robotInfo.ypos;
    qreal th = robotInfo.theta;
    _robot->setPos(x, -y);

    // Set rotation
    _robot->setRotation(-th + 90);
    advance();

    // Store pose info
    _robot->setPose(robotInfo);
}

void MapScene::updateRobotPath(Points *path)
{
    _path->setPath(path);
    _path->setPos(_path->getPos());
    advance();
}

void MapScene::setMode(Mode mode)
{
    _mode = mode;
}

MapScene::Mode MapScene::mode() const
{
    return _mode;
}

QGraphicsView * MapScene::viewFirst()
{
    return views().first();
}

void MapScene::wheelEvent(QGraphicsSceneWheelEvent* wheelEvent)
{
    qreal zoomFactor = 2.0;

    // Zoom in and out depending on wheel scroll direction
    QGraphicsView *view = viewFirst();
    if (wheelEvent->delta() > 0) //mousewheel up
    {
        view->scale(zoomFactor, zoomFactor);
    }
    else // mousewheel down
    {
        view->scale(1/zoomFactor, 1/zoomFactor);
    }

    advance();
}

void MapScene::mouseReleaseEvent(QGraphicsSceneMouseEvent * mouseEvent)
{
    QGraphicsView *view = viewFirst();
    if (view->dragMode() == QGraphicsView::ScrollHandDrag)
    {
        view->setDragMode(QGraphicsView::NoDrag);
    }
}

QList<QString> MapScene::goalList()
{
    return _goalNames;
}

void MapScene::clear()
{
    // Clear all map items on the scene and restart the map
    QGraphicsScene::clear();

#if 0 // FIXME: this causes a segfault for some reason
    delete[] _map;
#endif
}

ForbiddenRegion * MapScene::getMappedObstacle()
{
    return _mappedObstacle;
}
