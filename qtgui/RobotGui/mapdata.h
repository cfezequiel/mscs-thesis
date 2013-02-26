#ifndef MAPDATA_H
#define MAPDATA_H

#include <list>
#include <QMetaType>

using namespace std;

static const int STRLEN = 256;

//FIXME: refactor ArRobotInfo to use native C++ types
// Also rename it without the Ar
typedef struct
{
    int batVoltage;
    int xpos;
    int ypos;
    int theta;
    int forwardVel;
    int rotationVel;
    char status[STRLEN];
    char mode[STRLEN];
}
    ArRobotInfo;

struct Point
{
    int x;
    int y;
};

struct Points
{
    list<Point> data;
};

Q_DECLARE_METATYPE(ArRobotInfo)

#endif // MAPDATA_H
