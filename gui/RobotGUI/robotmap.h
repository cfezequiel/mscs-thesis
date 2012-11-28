#ifndef ROBOTMAP_H
#define ROBOTMAP_H

#include <QGraphicsScene>
#include <QFile>

class RobotMap : public QGraphicsScene
{
public:
    RobotMap();
    void load(const QFile &file);
};

#endif // ROBOTMAP_H
