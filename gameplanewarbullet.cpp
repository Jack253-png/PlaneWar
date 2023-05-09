#include "gameplanewarbullet.h"
#include "gameplanewar.h"
#include "vars.h"

GamePlaneWarBullet::GamePlaneWarBullet(QObject *parent) {
    GamePlaneWar *planeWarGame = (GamePlaneWar*) parent;
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
    this->moveBy(0, -5);
    if (this->y() < -this->pixmap().height()) {
        emit onDelete(this);
    }
}

void GamePlaneWarBullet::stopTimer() {
    this->eventTimer->stop();
}

void GamePlaneWarBullet::resumeTimer() {
    this->eventTimer->start();
}

void GamePlaneWarBullet::removeThis() {
    this->scene()->removeItem(this);
    delete this;
}
