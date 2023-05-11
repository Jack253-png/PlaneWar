#include "gameplanewarenemyplane.h"
#include "gameplanewarselfplane.h"
#include "gameplanewar.h"
#include "vars.h"
#include <QDebug>

GamePlaneWarEnemyPlane::GamePlaneWarEnemyPlane(QObject *parent) {
    GamePlaneWar *planeWarGame = (GamePlaneWar*) parent;
    this->game = planeWarGame;

    setState(SMALL_PLANE);
    this->eventTimer = new QTimer();
    this->eventTimer->setInterval(10);
    connect(this->eventTimer, SIGNAL(timeout()), this, SLOT(onEventTimerTimeout()));
    this->eventTimer->start();

    this->setPos(this->x(), -this->pixmap().height());
}

void GamePlaneWarEnemyPlane::onEventTimerTimeout() {
    this->moveBy(0, 5);
    if (this->y() > WINDOW_SIZE_HEIGHT) {
        ((GamePlaneWar*) this->game)->enemyPlanes->removeAll(this);
        delete this;
    }
}

void GamePlaneWarEnemyPlane::setState(PlaneState state) {
    this->state = state;
    switch (this->state) {
    case BIG_PLANE:
        setPixmap(IMG_ENEMY_PLANE_BIG);
        break;
    case MID_PLANE:
        setPixmap(IMG_ENEMY_PLANE_MID);
        break;
    case SMALL_PLANE:
        setPixmap(IMG_ENEMY_PLANE_SMALL);
        break;
    default:
        break;
    }
}

void GamePlaneWarEnemyPlane::stopTimer() {
    this->eventTimer->stop();
}

void GamePlaneWarEnemyPlane::resumeTimer() {
    this->eventTimer->start();
}

void GamePlaneWarEnemyPlane::removeThis() {
//    this->scene()->removeItem(this);
    delete this;
}
