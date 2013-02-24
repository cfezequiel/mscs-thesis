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

        // If object is a region
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
            else
            {
                cerr << "Unknown MapObject type = " << type << endl;
            }
        }
        // If object is a point
        else
        {
            x1 = pose.getX();
            y1 = pose.getY();
            obj = new Zone;

            // Set position
            obj->setPos(x1,-y1);
            obj->setRotation(-th);

            // Set colors
            type = (*i)->getType();
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
                cerr << "Unknown MapObject type = " << type << endl;
            }

            addItem(obj);
        }
    } // end render map objects

    // Store map
    _map = map;

    // Render robot
    _robot = new RobotObject;
    _robot->setZValue(2);
    addItem(_robot);

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

    // Set rotation
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
