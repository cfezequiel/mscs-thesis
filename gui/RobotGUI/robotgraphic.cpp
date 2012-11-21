#include <QPainter>
#include "robotgraphic.h"

RobotGraphic::RobotGraphic()
{
    _width = 40;
    _height = 44;
    _x = -_width / 2;
    _y = -_height / 2;
}

QRectF RobotGraphic::boundingRect() const
{
    qreal adjust = 2;
    return QRectF(_x - adjust, _y - adjust, _width + adjust , _height + adjust);
}

void RobotGraphic::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
    // Body
    qreal xRadius = 10;
    qreal yRadius = 10;
    painter->drawRoundedRect(_x, _y, _width, _height, xRadius, yRadius);

    // Pointer
    //TODO: draw triangle to indicate where robot is facing
}

