#ifndef GAMEPLANEWAR_H
#define GAMEPLANEWAR_H

#include "gameplanewarselfplane.h"
#include "gameplanewarenemyplane.h"
#include "gameplanewarbullet.h"
#include <QList>
#include <QObject>
#include <QTimer>
#include <QKeyEvent>
#include <QGraphicsScene>

class GamePlaneWar : public QObject
{
    Q_OBJECT
public:
    explicit GamePlaneWar(QObject *parent = nullptr);
    QList<GamePlaneWarBullet*> *bullets;
    QList<GamePlaneWarEnemyPlane*> *enemyPlanes;
    GamePlaneWarSelfPlane *selfPlane;
private:
    QGraphicsScene *scene;
    QTimer *eventTimer;

    void showBullets(bool isBlur);
    void cleanBullets();
    void showEnemys(bool isBlur);
    void cleanEnemys();
    bool isPausing = false;
signals:
    void onGamePause();

    void onOperationKeyPressed(QKeyEvent *event);
    void onOperationKeyReleased(QKeyEvent *event);
public slots:
    void onGameStart();
    void onGameEndCallback();
    void onGamePauseCallback();
    void onGameContinue();
    void onKeyPressed(QKeyEvent *event);
    void onKeyReleased(QKeyEvent *event);
    void onSelfPlaneBulleting();
    void onBulletDelete(GamePlaneWarBullet *bullet);
    void onEventTimerTimeout();
    void onEnemyDelete(GamePlaneWarEnemyPlane *enemy);
};

#endif // GAMEPLANEWAR_H
