#ifndef TELEOP_H
#define TELEOP_H

#include <QTimer>
#include <QObject>

#include "qarclient.h"

class Teleop : public QObject
{
    Q_OBJECT

public:
    explicit Teleop(QObject *parent = 0);
    void setTransRatio(double val);
    void setRotRatio(double val);
    void setScale(double val);
    void setEnabled(bool val);
    void reset();
    
signals:
    void sendInputs(double transRatio, double rotRatio, double scale);
    
public slots:
    void onTimeout();

private:
    double _transRatio;
    double _rotRatio;
    double _scale;
    bool _enabled;
    QTimer _timer;
    double _timeoutValTrans;
    double _timeoutValRot;
};

#endif // TELEOP_H
