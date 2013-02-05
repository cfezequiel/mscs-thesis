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

#include "mapscene.h"

using namespace std;

MapScene::MapScene(QObject *parent) :
    QGraphicsScene(parent),
    _mode(ModeView)
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

    // Render obstacles
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

    // Render map objects
    ArPose fromPose, toPose;
    MapObject *obj;
    list< ArMapObject *> *objects = map->getMapObjects();
    for (list< ArMapObject *>::iterator i = objects->begin();
         i != objects->end(); i++)
    {
        // If object is a region
        if ((*i)->hasFromTo())
        {
            fromPose = (*i)->getFromPose();
            toPose = (*i)->getToPose();
            fromPose.getPose(&x1, &y1, &th);
            toPose.getPose(&x2, &y2, &th);
            addRect(x1, -y1, fabs(x2-x1), fabs(y2-y1));
        }
        // If object is a point
        else
        {
            fromPose = (*i)->getPose();
            fromPose.getPose(&x1, &y1, &th);
            printf("fromPose(%f, %f)\n", x1, y1);
            obj = new MapObject;

            // Set position
            obj->setPos(x1,-y1);
            obj->setRotation(-th);

            // Set colors
            string type((*i)->getType());
            if (type == "GoalWithHeading")
            {
                obj->setLineColor(Qt::cyan);
                obj->setFillColor(Qt::darkCyan);
            }
            else if (type == "RobotHome")
            {
                obj->setLineColor(Qt::black);
                obj->setFillColor(QColor(0xbbeebb));
            }
            else
            {
                // do nothing; default settings
            }

            addItem(obj);
        }
    } // end render map objects

    // Render robot
    _robot = new RobotObject;
    _robot->setZValue(2);
    //_robot->setRotation(90);
    addItem(_robot);

    // Render path (should be invisible initially)
    _path = new PathObject;
    _path->setZValue(1);
    addItem(_path);
}

void MapScene::_modeAddObstacleRect(QPointF pos)
{
    // Add a rectangle centered at 'pos'
    // FIXME: hardcoded rect dimensions
    QGraphicsRectItem *rect = addRect(-393.0/2, -237.0/2, 393, 237);
    rect->setPos(pos);
}

void MapScene::mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    QPointF pos = mouseEvent->scenePos();

    // Do something depending on the mode
    switch (_mode)
    {
    case ModeView:
        // Do nothing
        break;
    case ModeAddObstacle:
        _modeAddObstacleRect(pos);
        break;
    default:
        // Do nothing, same as ModeView
        break;
    }
}

// FIXME: this should connect to RobotObject not the scene
void MapScene::updateRobotPose(ArRobotInfo *robotInfo)
{
    assert(robotInfo != NULL);
    assert(_robot != NULL);

    // Redraw the robot on the map based on robot telemetry info
    qreal x = robotInfo->xpos;
    qreal y = robotInfo->ypos;
    qreal th = robotInfo->theta;
    _robot->setPos(x, -y);

    // Set initial rotation
    _robot->setRotation(-th + 90);
    advance();
}

void MapScene::updateRobotPath(Points *path)
{
    _path->update(path);
    _path->setPos(_path->getPos());
    advance();
}

void printRobotInfo(ArRobotInfo *robotInfo)
{
    // Temp
    ArRobotInfo *r = robotInfo;
    cout << "Battery voltage: " << r->batVoltage << endl
         << "X-pos: " << r->xpos << endl
         << "Y-pos: " << r->ypos << endl
         << "theta: " << r->theta << endl
         << "Forward velocity: " << r->forwardVel << endl
         << "Rotation velocity: " << r->rotationVel << endl
         ;
}

void MapScene::setMode(Mode mode)
{
    _mode = mode;
}

MapScene::Mode MapScene::mode() const
{
    return _mode;
}
