#ifndef GAMEPLANEWARENEMYPLANE_H
#define GAMEPLANEWARENEMYPLANE_H

#include "blurableobject.h"
#include <QObject>
#include <QTimer>
#include <QGraphicsPixmapItem>

enum PlaneState {
    BIG_PLANE = 0,
    MID_PLANE = 1,
    SMALL_PLANE = 2
};

class GamePlaneWarEnemyPlane : public BlurableObject
{
    Q_OBJECT
public:
    explicit GamePlaneWarEnemyPlane(QObject *parent = nullptr);
    void setState(PlaneState state);
    void stopTimer();
    void resumeTimer();
    void removeThis();
    PlaneState state;
private:
    QTimer *eventTimer;
    QObject *game;
signals:
public slots:
    void onEventTimerTimeout();
};

#endif // GAMEPLANEWARENEMYPLANE_H
