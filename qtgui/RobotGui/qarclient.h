#ifndef QARCLIENT_H
#define QARCLIENT_H

#include <QObject>
#include "ArClient.h"

class QArClient : public QObject, public ArClient
{
    Q_OBJECT
public:
    explicit QArClient(QObject *parent = 0);
    
signals:
    void update();
    
public slots:

private:

};

#endif // QARCLIENT_H
