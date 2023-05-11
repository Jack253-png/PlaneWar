#include "gameplanewar.h"
#include "gamebasewindow.h"
#include "gameplanewarbullet.h"
#include "gameplanewarenemyplane.h"
#include "vars.h"
#include <QDebug>
#include <QList>
#include <cstdlib>

GamePlaneWar::GamePlaneWar(QObject *parent) {
    srand(rand());
    GameBaseWindow *base = (GameBaseWindow*) parent;

    this->scene = base->getScene();
    this->bullets = new QList<GamePlaneWarBullet*>();
    this->enemyPlanes = new QList<GamePlaneWarEnemyPlane*>();

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

    this->eventTimer = new QTimer();
    this->eventTimer->setInterval(1000);
    connect(this->eventTimer, SIGNAL(timeout()), this, SLOT(onEventTimerTimeout()));
}

void GamePlaneWar::onEventTimerTimeout() {
    GamePlaneWarEnemyPlane *plane = new GamePlaneWarEnemyPlane(this);
    int downLimit = WINDOW_SIZE_WIDTH - plane->pixmap().width();
    int selWidth = abs(rand() % downLimit);
    int planeType = abs(rand() % 10);
    switch (planeType) {
        default:
        case 0:
        case 1:
        case 2:
        case 3:
        case 4:
        case 5:
            plane->setState(SMALL_PLANE);
            break;
        case 6:
        case 7:
        case 8:
            plane->setState(MID_PLANE);
            break;
        case 9:
            plane->setState(BIG_PLANE);
            break;
    }
    plane->setPos(selWidth, plane->y());
    plane->show = true;
    this->scene->addItem(plane);
    this->enemyPlanes->append(plane);
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

void GamePlaneWar::showEnemys(bool isBlur) {
    for (int index = 0; index < this->enemyPlanes->size(); index++) {
        GamePlaneWarEnemyPlane *plane = this->enemyPlanes->at(index);
        plane->exceptedBlurRadius = isBlur ? 20 : 0;

        if (isBlur) plane->stopTimer();
        else plane->resumeTimer();
    }
}

void GamePlaneWar::cleanEnemys() {
    for (int index = 0; index < this->enemyPlanes->size(); index++) {
        GamePlaneWarEnemyPlane *plane = this->enemyPlanes->at(index);
//        this->scene->removeItem(bullet);
        this->enemyPlanes->removeAll(plane);
        plane->removeThis();
//        delete bullet;
    }
}

void GamePlaneWar::onGameStart() {
    this->eventTimer->start();
    this->selfPlane->show = true;
    this->selfPlane->exceptedBlurRadius = 0;
    this->selfPlane->eventTimer->start();
    this->cleanBullets();
    this->cleanEnemys();
    this->selfPlane->score = 0;
}

void GamePlaneWar::onGameEndCallback() {
    this->eventTimer->stop();
    this->selfPlane->show = false;
    this->selfPlane->exceptedBlurRadius = 20;
    this->selfPlane->eventTimer->stop();
    this->cleanBullets();
    this->cleanEnemys();
    this->selfPlane->score = 0;
}

void GamePlaneWar::onGamePauseCallback() {
    this->eventTimer->stop();
    this->selfPlane->exceptedBlurRadius = 20;
    this->selfPlane->eventTimer->stop();
    this->showBullets(true);
    this->showEnemys(true);
    isPausing = true;
}

void GamePlaneWar::onGameContinue() {
    this->eventTimer->start();
    this->selfPlane->exceptedBlurRadius = 0;
    this->selfPlane->eventTimer->start();
    this->showBullets(false);
    this->showEnemys(false);
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
    GamePlaneWarBullet *newBullet = new GamePlaneWarBullet(this);
    newBullet->selfPlane = this->selfPlane;
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

void GamePlaneWar::onEnemyDelete(GamePlaneWarEnemyPlane *enemy) {
    while (isPausing) {}
    enemy->stopTimer();
    this->enemyPlanes->removeAll(enemy);
    enemy->removeThis();
}
