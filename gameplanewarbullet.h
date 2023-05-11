#ifndef GAMEPLANEWARBULLET_H
#define GAMEPLANEWARBULLET_H

#include <QObject>
#include <QTimer>
#include <QGraphicsPixmapItem>
#include "blurableobject.h"
#include "gameplanewarselfplane.h"

class GamePlaneWarBullet : public BlurableObject
{
    Q_OBJECT
public:
    explicit GamePlaneWarBullet(QObject *parent = nullptr);
    void setIsEnemy(bool isEnemy);
    void stopTimer();
    void resumeTimer();
    void removeThis();
    GamePlaneWarSelfPlane *selfPlane;
private:
    QTimer *eventTimer;
    bool isEnemyBullet = false;
    QObject *game;
signals:
public slots:
    void onEventTimerTimeout();
};

#endif // GAMEPLANEWARBULLET_H
