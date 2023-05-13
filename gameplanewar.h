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

#define GAMETIME_SEC 30

class GamePlaneWar : public QObject
{
    Q_OBJECT
public:
    explicit GamePlaneWar(QObject *parent = nullptr);
    QList<GamePlaneWarBullet*> *bullets;
    QList<GamePlaneWarEnemyPlane*> *enemyPlanes;
    GamePlaneWarSelfPlane *selfPlane;
    QObject *window;

    void saveHistroy();
    int readHistroy();
    bool isTimeout();
private:
    QGraphicsScene *scene;
    QTimer *eventTimer;
    QTimer *timeDelTimer;

    void showBullets(bool isBlur);
    void cleanBullets();
    void showEnemys(bool isBlur);
    void cleanEnemys();
    void checkHistroyScore(bool isMenuEnd);
    bool isPausing = false;

    int time_eplased = 0;
signals:
    void onGamePause();
    void onGameFailedExit();
    void onGameTimeoutExit();

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
    void onGameEndedScoreTo0();
    void onTimeDelTimerTimeout();
};

#endif // GAMEPLANEWAR_H
