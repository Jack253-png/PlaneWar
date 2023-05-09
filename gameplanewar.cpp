#include "gameplanewar.h"
#include "gamebasewindow.h"
#include "gameplanewarbullet.h"
#include "vars.h"
#include <QDebug>

GamePlaneWar::GamePlaneWar(QObject *parent) {
    GameBaseWindow *base = (GameBaseWindow*) parent;

    this->scene = base->getScene();
    this->bullets = new QList<GamePlaneWarBullet*>();

    connect(base, SIGNAL(onKeyPressed(QKeyEvent*)), this, SLOT(onKeyPressed(QKeyEvent*)));
    connect(base, SIGNAL(onKeyReleased(QKeyEvent*)), this, SLOT(onKeyPressed(QKeyEvent*)));
    connect(base, SIGNAL(onGamePreStartComplete()), this, SLOT(onGameStart()));
    connect(this, SIGNAL(onGamePause()), base, SLOT(onGamePause()));
    connect(this, SIGNAL(onGamePause()), this, SLOT(onGamePauseCallback()));
    connect(base, SIGNAL(onGameContinue()), this, SLOT(onGameContinue()));
    connect(base, SIGNAL(onGamePreEnd()), this, SLOT(onGameEndCallback()));

    this->selfPlane = new GamePlaneWarSelfPlane(this);
    this->selfPlane->show = false;
    this->selfPlane->exceptedBlurRadius = 20;
    this->selfPlane->setPos(
                WINDOW_SIZE_WIDTH / 2 - this->selfPlane->pixmap().width() / 2,
                WINDOW_SIZE_HEIGHT - this->selfPlane->pixmap().height()
    );
    this->selfPlane->setFlag(QGraphicsItem::ItemIsFocusable);

    base->getScene()->addItem(this->selfPlane);

    connect(this, SIGNAL(onOperationKeyPressed(QKeyEvent*)), this->selfPlane, SLOT(onKeyPressed(QKeyEvent*)));
    connect(this, SIGNAL(onOperationKeyReleased(QKeyEvent*)), this->selfPlane, SLOT(onKeyReleased(QKeyEvent*)));
    connect(this->selfPlane, SIGNAL(onBulleting()), this, SLOT(onSelfPlaneBulleting()));
}

void GamePlaneWar::showBullets(bool isBlur) {
    for (int index = 0; index < this->bullets->size(); index++) {
        GamePlaneWarBullet *bullet = this->bullets->at(index);
        bullet->exceptedBlurRadius = isBlur ? 20 : 0;

        if (isBlur) bullet->stopTimer();
        else bullet->resumeTimer();
    }
}

void GamePlaneWar::cleanBullets() {
    for (int index = 0; index < this->bullets->size(); index++) {
        GamePlaneWarBullet *bullet = this->bullets->at(index);
//        this->scene->removeItem(bullet);
        this->bullets->removeAll(bullet);
        bullet->removeThis();
//        delete bullet;
    }
}

void GamePlaneWar::onGameStart() {
    this->selfPlane->show = true;
    this->selfPlane->exceptedBlurRadius = 0;
    this->selfPlane->eventTimer->start();
    this->cleanBullets();
}

void GamePlaneWar::onGameEndCallback() {
    this->selfPlane->show = false;
    this->selfPlane->exceptedBlurRadius = 20;
    this->selfPlane->eventTimer->stop();
    this->cleanBullets();
}

void GamePlaneWar::onGamePauseCallback() {
    this->selfPlane->exceptedBlurRadius = 20;
    this->selfPlane->eventTimer->stop();
    this->showBullets(true);
    isPausing = true;
}

void GamePlaneWar::onGameContinue() {
    this->selfPlane->exceptedBlurRadius = 0;
    this->selfPlane->eventTimer->start();
    this->showBullets(false);
    isPausing = false;
}

void GamePlaneWar::onKeyPressed(QKeyEvent *event) {
    if (event->key() == Qt::Key_Escape) emit onGamePause();
    else if (
             event->key() == Qt::Key_W ||
             event->key() == Qt::Key_S ||
             event->key() == Qt::Key_A ||
             event->key() == Qt::Key_D ||
             event->key() == Qt::Key_Up ||
             event->key() == Qt::Key_Down ||
             event->key() == Qt::Key_Left ||
             event->key() == Qt::Key_Right ||
             event->key() == Qt::Key_Space) emit onOperationKeyPressed(event);
}

void GamePlaneWar::onKeyReleased(QKeyEvent *event) {
    if (
            event->key() == Qt::Key_W ||
            event->key() == Qt::Key_S ||
            event->key() == Qt::Key_A ||
            event->key() == Qt::Key_D ||
            event->key() == Qt::Key_Up ||
            event->key() == Qt::Key_Down ||
            event->key() == Qt::Key_Left ||
            event->key() == Qt::Key_Right ||
            event->key() == Qt::Key_Space) emit onOperationKeyReleased(event);
}

void GamePlaneWar::onSelfPlaneBulleting() {
    GamePlaneWarBullet *newBullet = new GamePlaneWarBullet();
    connect(newBullet, SIGNAL(onDelete(GamePlaneWarBullet*)), this, SLOT(onBulletDelete(GamePlaneWarBullet*)));
    newBullet->show = true;
    this->bullets->append(newBullet);
    newBullet->setPos(this->selfPlane->x() + this->selfPlane->pixmap().width() / 2, this->selfPlane->y());

    this->scene->addItem(newBullet);
}

void GamePlaneWar::onBulletDelete(GamePlaneWarBullet *bullet) {
    while (isPausing) {}
    bullet->stopTimer();
//    this->scene->removeItem(bullet);
    this->bullets->removeAll(bullet);
    bullet->removeThis();
}
