#include "blurableobject.h"
#include <QGraphicsBlurEffect>
#include <QTimer>

BlurableObject::BlurableObject(QObject *parent)
{
    this->show = false;
    this->currentBlurRadius = 0;
    this->exceptedBlurRadius = 0;

    this->timer = new QTimer();
    this->timer->setInterval(10);
    connect(this->timer, SIGNAL(timeout()), this, SLOT(onShowStateTimerTimeout()));
    this->timer->start();

    this->blurTimer = new QTimer();
    this->blurTimer->setInterval(10);
    connect(this->blurTimer, SIGNAL(timeout()), this, SLOT(onBlurTimerTimeout()));
    this->blurTimer->start();
}

void BlurableObject::onShowStateTimerTimeout() {
    double o = this->opacity();
    if (show && o < 1) this->setOpacity(o + 0.05);
    if (!show && o > 0) this->setOpacity(o - 0.05);
}

void BlurableObject::onBlurTimerTimeout() {
    int current = this->currentBlurRadius;
    int excepted = this->exceptedBlurRadius;
    if (current == excepted) return;

    current = current + (current > excepted ? -1 : 1);
    this->currentBlurRadius = current;

    QGraphicsBlurEffect *effect = new QGraphicsBlurEffect();
    effect->setBlurRadius(current);

    this->setGraphicsEffect(effect);
}
