#include "gamebasewindow.h"
#include "graphicsbutton.h"
#include "gameplanewar.h"
#include "vars.h"
#include <QApplication>
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <QDebug>
#include "windows.h"

GameBaseWindow::GameBaseWindow(QObject *parent) {
    QApplication *app = (QApplication*) parent;
    this->app = app;

    this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    this->scene = new QGraphicsScene();
    this->scene->setItemIndexMethod(QGraphicsScene::NoIndex);
    this->setScene(scene);
    this->setWindowTitle("飞机大战 Cpp Demo");

    this->planeWar = new GamePlaneWar(this);

    this->baseTimer = new QTimer();
    this->baseTimer->setInterval(100);
    connect(this->baseTimer, SIGNAL(timeout()), this, SLOT(onBaseTimerTimeout()));
    this->baseTimer->start();

    this->scene->setFocus();
    this->setFocus();
}

void GameBaseWindow::onBaseTimerTimeout() {
    app->processEvents();
}

void GameBaseWindow::initComponments() {
    this->setFixedSize(WINDOW_SIZE_WIDTH, WINDOW_SIZE_HEIGHT);

    this->backgroundItem = new BlurableObject();
    this->backgroundItem->setZValue(0);
    this->backgroundItem->setPixmap(IMG_BACKGROUND);
    this->backgroundItem->show = true;
    this->backgroundItem->exceptedBlurRadius = 20;
    this->scene->setBackgroundBrush(QBrush(IMG_BACKGROUND));

    this->gameTitle = new GraphicsButton(this->scene);
    this->gameTitle->setText("飞机大战");
    this->gameTitle->setPos(WINDOW_SIZE_WIDTH / 2 - 50, 50, 100, 60);
    this->gameTitle->showBg = false;

    this->gameStartButton = new GraphicsButton(this->scene);
    this->gameStartButton->setText("开始游戏");
    this->gameStartButton->setPos(WINDOW_SIZE_WIDTH / 2 - 100, 150, 200, 60);

    this->pauseTitle = new GraphicsButton(this->scene);
    this->pauseTitle->setText("暂停菜单");
    this->pauseTitle->setPos(WINDOW_SIZE_WIDTH / 2 - 50, 50, 100, 60);
    this->pauseTitle->showBg = false;
    this->pauseTitle->setVisible(false);

    this->pauseContinue = new GraphicsButton(this->scene);
    this->pauseContinue->setText("继续");
    this->pauseContinue->setPos(WINDOW_SIZE_WIDTH / 2 - 100, 150, 200, 60);
    this->pauseContinue->setVisible(false);

    this->pauseExit = new GraphicsButton(this->scene);
    this->pauseExit->setText("退出");
    this->pauseExit->setPos(WINDOW_SIZE_WIDTH / 2 - 100, 250, 200, 60);
    this->pauseExit->setVisible(false);

    this->scoreLabel = new GraphicsButton(this->scene);
    this->scoreLabel->setPos(0, 0, WINDOW_SIZE_WIDTH, 60);
    this->scoreLabel->setText("");
    this->scoreLabel->showBg = false;

    connect(this->gameStartButton, SIGNAL(onClicked()), this, SLOT(onGamePreStart()));
    connect(this->pauseContinue, SIGNAL(onClicked()), this, SLOT(onGamePreContinue()));
    connect(this->pauseExit, SIGNAL(onClicked()), this, SLOT(onGameEnd()));

    connect(this->planeWar, SIGNAL(onGameFailedExit()), this, SLOT(onGameEnd()));
    connect(this->planeWar, SIGNAL(onGameTimeoutExit()), this, SLOT(onGameEnd()));

    this->scene->addItem(this->backgroundItem);
    this->scene->addItem(this->gameTitle);
    this->scene->addItem(this->gameStartButton);
    this->scene->addItem(this->pauseTitle);
    this->scene->addItem(this->pauseContinue);
    this->scene->addItem(this->pauseExit);
    this->scene->addItem(this->scoreLabel);
}

void GameBaseWindow::setScore(int i) {
    this->scoreLabel->setText("分数: " + QString::number(i) + " 分");
    this->score = i;
}

void GameBaseWindow::setCustomMessage(QString string) {
    this->customMessage = string;
}

void GameBaseWindow::run() {
    this->show();

    // solve problem of key event failed (?
    emit this->gameStartButton->onClicked();
    this->gamePausing = true;
    this->hidePauseMenu(false);
    emit this->pauseExit->onClicked();
    this->scoreLabel->setText("");
}

QGraphicsScene* GameBaseWindow::getScene() {
    return this->scene;
}

void GameBaseWindow::keyPressEvent(QKeyEvent *event) {
    if (gameRunning && !gamePausing) {
        emit onKeyPressed(event);
    }
}

void GameBaseWindow::keyReleaseEvent(QKeyEvent *event) {
    if (gameRunning && !gamePausing) {
        emit onKeyReleased(event);
    }
}

void GameBaseWindow::mouseReleaseEvent(QMouseEvent *event) {

}

void GameBaseWindow::hideGameMenu(bool isHide) {
    this->gameTitle->setVisible(!isHide);
    this->gameStartButton->setVisible(!isHide);
}

void GameBaseWindow::hidePauseMenu(bool isHide) {
    this->pauseTitle->setVisible(!isHide);
    this->pauseContinue->setVisible(!isHide);
    this->pauseExit->setVisible(!isHide);
}

void GameBaseWindow::onGameStart() {

}

void GameBaseWindow::onGameEnd() {
    emit onGamePreEnd();
    this->gameRunning = false;
    this->gamePausing = false;
    hidePauseMenu(true);
    hideGameMenu(false);
    this->backgroundItem->exceptedBlurRadius = 20;
    if (this->customMessage != NULL) {
        this->scoreLabel->setText(this->customMessage);
    }
    else {
        this->scoreLabel->setText("您已退出游戏，得分为 " + QString::number(this->score) + " 分");
    }
    this->scene->setFocus();
    this->setFocus();
}

void GameBaseWindow::onGamePause() {
    this->gamePausing = true;
    this->backgroundItem->exceptedBlurRadius = 20;
    hidePauseMenu(false);
    this->scene->setFocus();
    this->setFocus();
}

void GameBaseWindow::onGamePreStart() {
    this->gameRunning = true;
    hideGameMenu(true);
    this->backgroundItem->exceptedBlurRadius = 0;
    emit onGamePreStartComplete();
    this->scene->setFocus();
    this->setFocus();

    QGraphicsRectItem *item = new QGraphicsRectItem();
    this->scene->addItem(item);
    delete item;
}

void GameBaseWindow::onGamePreContinue() {
    this->gamePausing = false;
    this->backgroundItem->exceptedBlurRadius = 0;
    hidePauseMenu(true);
    emit onGameContinue();
    this->scene->setFocus();
    this->setFocus();
}
