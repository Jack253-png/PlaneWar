#include "gamebasewindow.h"
#include "graphicsbutton.h"
#include "gameplanewar.h"
#include "vars.h"
#include <QApplication>
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>

GameBaseWindow::GameBaseWindow(QObject *parent) {
    QApplication *app = (QApplication*) parent;

    this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    this->scene = new QGraphicsScene();
    this->scene->setItemIndexMethod(QGraphicsScene::NoIndex);
    this->setScene(scene);
    this->setWindowTitle("飞机大战 Cpp Demo");

    this->planeWar = new GamePlaneWar(this);
}

void GameBaseWindow::initComponments() {
    this->setFixedSize(WINDOW_SIZE_WIDTH, WINDOW_SIZE_HEIGHT);

    this->backgroundItem = new BlurableObject();
    this->backgroundItem->setZValue(0);
    this->backgroundItem->setPixmap(IMG_BACKGROUND);
    this->backgroundItem->show = true;
    this->backgroundItem->exceptedBlurRadius = 20;

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

    connect(this->gameStartButton, SIGNAL(onClicked()), this, SLOT(onGamePreStart()));
    connect(this->pauseContinue, SIGNAL(onClicked()), this, SLOT(onGamePreContinue()));
    connect(this->pauseExit, SIGNAL(onClicked()), this, SLOT(onGameEnd()));

    this->scene->addItem(this->backgroundItem);
    this->scene->addItem(this->gameTitle);
    this->scene->addItem(this->gameStartButton);
    this->scene->addItem(this->pauseTitle);
    this->scene->addItem(this->pauseContinue);
    this->scene->addItem(this->pauseExit);
}

void GameBaseWindow::run() {
    this->show();
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
}

void GameBaseWindow::onGamePause() {
    this->gamePausing = true;
    this->backgroundItem->exceptedBlurRadius = 20;
    hidePauseMenu(false);
}

void GameBaseWindow::onGamePreStart() {
    this->gameRunning = true;
    hideGameMenu(true);
    this->backgroundItem->exceptedBlurRadius = 0;
    emit onGamePreStartComplete();
}

void GameBaseWindow::onGamePreContinue() {
    this->gamePausing = false;
    this->backgroundItem->exceptedBlurRadius = 0;
    hidePauseMenu(true);
    emit onGameContinue();
}
