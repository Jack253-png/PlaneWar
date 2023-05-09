#ifndef GAMEPLANEWAR_H
#define GAMEPLANEWAR_H

#include "gameplanewarselfplane.h"
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
private:
    QGraphicsScene *scene;
    GamePlaneWarSelfPlane *selfPlane;
    QList<GamePlaneWarBullet*> *bullets;

    void showBullets(bool isBlur);
    void cleanBullets();
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
};

#endif // GAMEPLANEWAR_H
