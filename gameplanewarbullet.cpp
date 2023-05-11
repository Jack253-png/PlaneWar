#include "gameplanewarbullet.h"
#include "gameplanewarselfplane.h"
#include "gameplanewarenemyplane.h"
#include "gameplanewar.h"
#include "vars.h"
#include <QList>
#include <QDebug>

GamePlaneWarBullet::GamePlaneWarBullet(QObject *parent) {
    GamePlaneWar *planeWarGame = (GamePlaneWar*) parent;
    this->game = planeWarGame;
    this->setPixmap(IMG_BULLET_1);

    this->eventTimer = new QTimer();
    this->eventTimer->setInterval(10);
    connect(this->eventTimer, SIGNAL(timeout()), this, SLOT(onEventTimerTimeout()));
    this->eventTimer->start();

    this->setZValue(2);
}

void GamePlaneWarBullet::setIsEnemy(bool isEnemy) {
    this->isEnemyBullet = isEnemy;
    this->setPixmap(isEnemy ? IMG_BULLET_2 : IMG_BULLET_1);
}

void GamePlaneWarBullet::onEventTimerTimeout() {
    this->moveBy(0, isEnemyBullet ? 5 : -5);
    QList<QGraphicsItem *> coll = collidingItems();
    if (coll.size() > 1) {
        for (int index = 1; index < coll.size(); index++) {
            if (typeid(*coll[index]) == typeid(GamePlaneWarSelfPlane)) return;
            if (typeid(*coll[index]) == typeid(GamePlaneWarEnemyPlane)) {
                GamePlaneWarEnemyPlane *enemy = (GamePlaneWarEnemyPlane*) (coll[index]);
                int score = 0;
                switch (enemy->state) {
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
                ((GamePlaneWar*) this->game)->selfPlane->score += score;
                ((GamePlaneWar*) this->game)->selfPlane->updateScore();

                ((GamePlaneWar*) this->game)->bullets->removeAll(this);
                ((GamePlaneWar*) this->game)->enemyPlanes->removeAll(enemy);
                delete this;
                delete enemy;
                return;
            }
        }
    }
    if (this->y() < -this->pixmap().height() || this->y() > WINDOW_SIZE_HEIGHT) {
        ((GamePlaneWar*) this->game)->bullets->removeAll(this);
        delete this;
    }
}

void GamePlaneWarBullet::stopTimer() {
    this->eventTimer->stop();
}

void GamePlaneWarBullet::resumeTimer() {
    this->eventTimer->start();
}

void GamePlaneWarBullet::removeThis() {
//    this->scene()->removeItem(this);
    delete this;
}
