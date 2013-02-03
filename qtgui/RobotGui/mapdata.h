#ifndef MAPDATA_H
#define MAPDATA_H

#include <list>
#include <QMetaType>

using namespace std;

static const int STRLEN = 256;

//FIXME: refactor ArRobotInfo to use native C++ types
// Also rename it without the Ar
struct ArRobotInfo
{
    int batVoltage;
    int xpos;
    int ypos;
    int theta;
    int forwardVel;
    int rotationVel;
    char status[STRLEN];
    char mode[STRLEN];
};

struct Point
{
    int x;
    int y;
};

Q_DECLARE_METATYPE(Point)
Q_DECLARE_METATYPE(list<Point>)

#endif // MAPDATA_H
