#include "gameplanewarselfplane.h"
#include "gameplanewarenemyplane.h"
#include "gameplanewarbullet.h"
#include "gameplanewar.h"
#include <QPixmap>
#include <QGraphicsBlurEffect>
#include <QGraphicsScene>
#include <QDebug>
#include <QPointF>
#include "vars.h"

GamePlaneWarSelfPlane::GamePlaneWarSelfPlane(QObject *parent) {
    GamePlaneWar *planeWarGame = (GamePlaneWar*) parent;
    this->game = planeWarGame;

    this->setZValue(2);

    this->setPixmap(IMG_MYPLANE_1);
    this->setOpacity(0);

    this->eventTimer = new QTimer();
    this->eventTimer->setInterval(2);
    connect(this->eventTimer, SIGNAL(timeout()), this, SLOT(onEventTimerTimeout()));
    this->eventTimer->start();
}

void GamePlaneWarSelfPlane::updateScore() {
    qDebug() << score;
}

void GamePlaneWarSelfPlane::onKeyPressed(QKeyEvent *event) {
    this->setFocus();
    int k = event->key();
    if (k == Qt::Key_W || k == Qt::Key_Up) up = event->type() == QKeyEvent::Type::KeyPress;
    if (k == Qt::Key_S || k == Qt::Key_Down) down = event->type() == QKeyEvent::Type::KeyPress;
    if (k == Qt::Key_A || k == Qt::Key_Left) left = event->type() == QKeyEvent::Type::KeyPress;
    if (k == Qt::Key_D || k == Qt::Key_Right) right = event->type() == QKeyEvent::Type::KeyPress;
    if (k == Qt::Key_Space) {
        if (event->type() == QKeyEvent::Type::KeyPress && !event->isAutoRepeat()) emit onBulleting();
    }
}

void GamePlaneWarSelfPlane::onKeyReleased(QKeyEvent *event) {
    this->setFocus();
}

void GamePlaneWarSelfPlane::onEventTimerTimeout() {
    int half_width = this->pixmap().width() / 2;
    int half_height = this->pixmap().height() / 2;

    if (up && y() >= 0) setPos(x(), y() - 1);
    if (down && y() <= WINDOW_SIZE_HEIGHT - half_height) setPos(x(), y() + 1);
    if (left && x() >= -half_width) moveBy(-1, 0);
    if (right && x() <= WINDOW_SIZE_WIDTH - half_width) setPos(x() + 1, y());

    image = !image;

    this->setPixmap(image ? IMG_MYPLANE_1 : IMG_MYPLANE_2);


    QList<QGraphicsItem *> coll = collidingItems();
    if (coll.size() > 1) {
        for (int index = 1; index < coll.size(); index++) {
            if (typeid(*coll[index]) == typeid(GamePlaneWarEnemyPlane)) {
                GamePlaneWarEnemyPlane *ene = (GamePlaneWarEnemyPlane*) coll[index];
                int score = 0;
                switch (ene->state) {
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
                if (this->score >= score * 2) {
                    this->score -= score * 2;
                }
                else {
                    this->score = 0;
                }
                this->updateScore();
                ((GamePlaneWar*) this->game)->enemyPlanes->removeAll(ene);
                delete ene;
            }
        }
    }
}
