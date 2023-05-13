#include "gameplanewar.h"
#include "gamebasewindow.h"
#include "gameplanewarbullet.h"
#include "gameplanewarenemyplane.h"
#include "vars.h"
#include <QDebug>
#include <QList>
#include <cstdlib>
#include <iostream>
#include "windows.h"
#include <QFile>
#include <QIODevice>

GamePlaneWar::GamePlaneWar(QObject *parent) {
    srand(rand());
    GameBaseWindow *base = (GameBaseWindow*) parent;
    this->window = base;

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
    connect(this->selfPlane, SIGNAL(onScoreTo0()), this, SLOT(onGameEndedScoreTo0()));
    connect(base, SIGNAL(onGamePreEnd()), this->selfPlane, SLOT(onGameEndCallback()));

    this->eventTimer = new QTimer();
    this->eventTimer->setInterval(1000);
    connect(this->eventTimer, SIGNAL(timeout()), this, SLOT(onEventTimerTimeout()));

    this->timeDelTimer = new QTimer();
    this->timeDelTimer->setInterval(1000);
    connect(this->timeDelTimer, SIGNAL(timeout()), this, SLOT(onTimeDelTimerTimeout()));
}

void GamePlaneWar::saveHistroy() {
    QFile *file = new QFile("planewar_save");
    file->open(QIODevice::ReadWrite);
    file->write(QString::number(this->selfPlane->score).toUtf8());
    file->close();
}

int GamePlaneWar::readHistroy() {
    QFile *file = new QFile("planewar_save");
    file->open(QIODevice::ReadWrite);
    QString result = file->readAll();
    file->close();

    bool ok;
    return result.toInt(&ok, 10);
}

void GamePlaneWar::checkHistroyScore(bool isMenuEnd) {
    int last = readHistroy();
    bool uppedHistroy = this->selfPlane->score > readHistroy();
    if (uppedHistroy) this->saveHistroy();

    QString reason = isMenuEnd ? "您已退出游戏" : "时间到";
    QString a = isMenuEnd ? "" : "您";
    QString score = ", " + a + "得了 " + QString::number(this->selfPlane->score) + " 分";
    QString histroy = uppedHistroy ? ", 已超过记录 " + QString::number(last) + " 分" : "";

    ((GameBaseWindow*) this->window)->setCustomMessage(
                reason + score + histroy
    );
}

bool GamePlaneWar::isTimeout() {
    return this->time_eplased > GAMETIME_SEC;
}

void GamePlaneWar::onTimeDelTimerTimeout() {
    this->time_eplased += 1;
    if (isTimeout()) {
        checkHistroyScore(false);
        emit onGameTimeoutExit();
    }
}

void GamePlaneWar::onGameEndedScoreTo0() {
    this->time_eplased = GAMETIME_SEC + 1;
    ((GameBaseWindow*) this->window)->setCustomMessage("分数归零, 游戏结束");
    emit onGameFailedExit();
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
        bullet->show = false;
        bullet->stopTimer();
        this->bullets->removeAll(bullet);
        delete bullet;
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
        plane->show = false;
        plane->stopTimer();
        this->enemyPlanes->removeAll(plane);
        delete plane;
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
    this->selfPlane->updateScore();
    this->time_eplased = 0;
    this->timeDelTimer->start();
    this->selfPlane->setPos(
                WINDOW_SIZE_WIDTH / 2 - this->selfPlane->pixmap().width() / 2,
                WINDOW_SIZE_HEIGHT - this->selfPlane->pixmap().height()
    );
}

void GamePlaneWar::onGameEndCallback() {
    if (!isTimeout()) checkHistroyScore(true);
    this->eventTimer->stop();
    this->selfPlane->show = false;
    this->selfPlane->exceptedBlurRadius = 20;
    this->selfPlane->eventTimer->stop();
    this->cleanBullets();
    this->cleanEnemys();
    this->cleanBullets();
    this->cleanEnemys();
    this->selfPlane->score = 0;
    this->time_eplased = 0;
    this->timeDelTimer->stop();
}

void GamePlaneWar::onGamePauseCallback() {
    this->eventTimer->stop();
    this->selfPlane->exceptedBlurRadius = 20;
    this->selfPlane->eventTimer->stop();
    this->showBullets(true);
    this->showEnemys(true);
    isPausing = true;
    this->timeDelTimer->stop();
}

void GamePlaneWar::onGameContinue() {
    this->eventTimer->start();
    this->selfPlane->exceptedBlurRadius = 0;
    this->selfPlane->eventTimer->start();
    this->showBullets(false);
    this->showEnemys(false);
    isPausing = false;
    this->timeDelTimer->start();
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
    this->bullets->removeAll(bullet);
    bullet->removeThis();
}

void GamePlaneWar::onEnemyDelete(GamePlaneWarEnemyPlane *enemy) {
    while (isPausing) {}
    enemy->stopTimer();
    this->enemyPlanes->removeAll(enemy);
    enemy->removeThis();
}
