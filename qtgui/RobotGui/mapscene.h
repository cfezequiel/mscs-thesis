#ifndef MAPSCENE_H
#define MAPSCENE_H

#include "Aria.h"
#include "ArNetworking.h"
#include <QMainWindow>
#include <QGraphicsScene>
#include <QRectF>
#include <QPoint>
#include <QCursor>

class MapScene : public QGraphicsScene
{
    Q_OBJECT

public:
    explicit MapScene(QObject *parent = 0);
    void renderMap(ArMap *map);
    
protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent);

signals:
    
public slots:

private:
    
};

#endif // MAPSCENE_H
