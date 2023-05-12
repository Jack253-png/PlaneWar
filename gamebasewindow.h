#ifndef GAMEBASEWINDOW_H
#define GAMEBASEWINDOW_H

#include "vars.h"
#include "graphicsbutton.h"
#include "gameplanewar.h"
#include <QTimer>
#include <QObject>
#include <QKeyEvent>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsRectItem>
#include <QGraphicsBlurEffect>
#include <blurableobject.h>
#include <QApplication>

class GameBaseWindow : public QGraphicsView {
    Q_OBJECT
public:
    explicit GameBaseWindow(QObject *parent = nullptr);
    void initComponments();
    void run();
    QGraphicsScene* getScene();
    bool gameRunning;
    bool gamePausing;

    void setScore(int i);
    void setCustomMessage(QString string);
protected:
    void keyPressEvent(QKeyEvent *event);
    void keyReleaseEvent(QKeyEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
private:
    QString customMessage;
    int score = 0;
    GraphicsButton *scoreLabel;
    QGraphicsScene *scene;
    QGraphicsPixmapItem* createBg();
    QTimer *baseTimer;
    QApplication *app;
    void hideGameMenu(bool isHide);
    void hidePauseMenu(bool isHide);

    GraphicsButton *gameTitle;
    GraphicsButton *gameStartButton;
    GraphicsButton *pauseTitle;
    GraphicsButton *pauseContinue;
    GraphicsButton *pauseExit;

    BlurableObject *backgroundItem;

    GamePlaneWar *planeWar;
signals:
    void onKeyPressed(QKeyEvent *event);
    void onKeyReleased(QKeyEvent *event);
    void onGamePreStartComplete();
    void onGameContinue();
    void onGamePreEnd();
public slots:
    void onGameStart();
    void onGameEnd();
    void onGamePause();
    void onGamePreStart();
    void onGamePreContinue();
    void onBaseTimerTimeout();
};

#endif // GAMEBASEWINDOW_H
