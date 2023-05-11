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
    QList<QGraphicsItem *> coll = collidingItems();
    if (coll.size() > 1) {
        for (int index = 1; index < coll.size(); index++) {
            if (typeid(*coll[index]) == typeid(GamePlaneWarSelfPlane)) {
                int score = 0;
                switch (this->state) {
                case BIG_PLANE:
                    score = 10;
                    break;
                case MID_PLANE:
                    score = 3;
                    break;
                case SMALL_PLANE:
                    score = 1;
                    break;
                default:
                    break;
                }
                if (((GamePlaneWar*) this->game)->selfPlane->score >= score * 2) {
                    ((GamePlaneWar*) this->game)->selfPlane->score -= score * 2;
                }
                else {
                    ((GamePlaneWar*) this->game)->selfPlane->score = 0;
                }
                ((GamePlaneWar*) this->game)->selfPlane->updateScore();
                ((GamePlaneWar*) this->game)->enemyPlanes->removeAll(this);
                delete this;
            }
        }
    }
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
    this->scene()->removeItem(this);
    delete this;
}
