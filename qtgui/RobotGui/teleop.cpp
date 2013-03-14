#include "teleop.h"

Teleop::Teleop(QObject *parent) :
    QObject(parent)
{
    _enabled = false;
    _timer.setSingleShot(true);
    _timeoutValTrans = 200;
    _timeoutValRot = 100;
    QObject::connect(&_timer, SIGNAL(timeout()), this, SLOT(onTimeout()));
    reset();
}

void Teleop::setTransRatio(double val)
{
    if (_enabled && !_timer.isActive())
    {
        _timer.start(_timeoutValTrans);
    }

    _transRatio = val;
}

void Teleop::setRotRatio(double val)
{
    if (_enabled && !_timer.isActive())
    {
        _timer.start(_timeoutValRot);
    }

    _rotRatio = val;
}

void Teleop::setScale(double val)
{
    _scale = val;
}

void Teleop::setEnabled(bool val)
{
    if (!val)
    {
        _timer.stop();
    }

    _enabled = val;
}

void Teleop::onTimeout()
{
    sendInputs(_transRatio, _rotRatio, _scale);
    reset();
}


void Teleop::reset()
{
    _transRatio = 0;
    _rotRatio = 0;
    _scale = 50;
}
