#ifndef GAMEPLANEWARSELFPLANE_H
#define GAMEPLANEWARSELFPLANE_H

#include <QObject>
#include <QTimer>
#include <QGraphicsPixmapItem>
#include <QGraphicsScene>
#include <QKeyEvent>
#include "blurableobject.h"

class GamePlaneWarSelfPlane : public BlurableObject
{
    Q_OBJECT
public:
    explicit GamePlaneWarSelfPlane(QObject *parent = nullptr);
    QTimer *eventTimer;
    int score = 0;

    void updateScore();
private:
    bool up = false;
    bool down = false;
    bool left = false;
    bool right = false;

    bool image = false;
    QObject *game;
signals:
    void onBulleting();
    void onScoreTo0();
public slots:
    void onKeyPressed(QKeyEvent *event);
    void onKeyReleased(QKeyEvent *event);
    void onEventTimerTimeout();
    void onGameEndCallback();
};

#endif // GAMEPLANEWARSELFPLANE_H
