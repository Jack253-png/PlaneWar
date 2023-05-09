#include "graphicsbutton.h"
#include <QBrush>
#include <QColor>
#include <QGraphicsScene>
#include <QGraphicsSimpleTextItem>
#include <QDebug>
#include <QRectF>
#include <QFontMetrics>
#include <QTimer>
#include <QCursor>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsSceneHoverEvent>

GraphicsButton::GraphicsButton(QObject *basescene) {
    this->exceptedOpacity = 0.15;
    this->basescene = (QGraphicsScene*) basescene;
    this->internalText = new QGraphicsSimpleTextItem();
//    this->setCursor(QCursor(Qt::CursorShape::OpenHandCursor));

    this->setBrush(QBrush(QColor(0, 0, 0)));
    this->setOpacity(this->exceptedOpacity);

    this->timer = new QTimer();
    this->timer->setInterval(10);
    connect(this->timer, SIGNAL(timeout()), this, SLOT(onOpacityUpdate()));
    this->timer->start();

    this->internalText->setFont(DEFAULT_FONT);

    this->basescene->addItem(this->internalText);
    setZValue(1);
    this->internalText->setZValue(1);
    this->childItems().append(this->internalText);
    this->setAcceptHoverEvents(true);
    this->setAcceptTouchEvents(true);
    this->setPen(QPen(Qt::NoPen));

    this->setFlag(QGraphicsItem::ItemIsFocusable, false);
}

void GraphicsButton::setPos(int x, int y, int w, int h) {
    this->setRect(x, y, w, h);
    this->updateText();
}

void GraphicsButton::updateText() {
    QRectF f = this->rect();
    QSize *textSiz = this->calcPos();
    int x = f.x() + f.width() / 2 - textSiz->width() / 2;
    int y = f.y() + f.height() / 2 - textSiz->height() / 2 - 5;

    this->internalText->setPos(x, y);
}

QSize* GraphicsButton::calcPos() {
    QFontMetricsF fm(this->internalText->font());
    qreal absW = fm.width(this->internalText->text().toUpper());
    qreal absH = fm.ascent();

    return new QSize(absW, absH);
}

void GraphicsButton::mousePressEvent(QGraphicsSceneMouseEvent *event) {
    QGraphicsItem::mousePressEvent(event);
    event->accept();
    emit onClicked();
    this->exceptedOpacity = 0.35;
}

void GraphicsButton::mouseReleaseEvent(QGraphicsSceneMouseEvent *event) {
    QGraphicsItem::mouseReleaseEvent(event);
    event->accept();
    qDebug() << event;
}

void GraphicsButton::hoverEnterEvent(QGraphicsSceneHoverEvent *event) {
    QGraphicsItem::hoverEnterEvent(event);
    event->accept();
    this->exceptedOpacity = 0.25;
}

void GraphicsButton::hoverLeaveEvent(QGraphicsSceneHoverEvent *event) {
    QGraphicsItem::hoverLeaveEvent(event);
    event->accept();
    this->exceptedOpacity = 0.15;
}

void GraphicsButton::onOpacityUpdate() {
    double current = this->opacity();
    double excepted = this->exceptedOpacity;

    double delta = current - excepted;
    if (delta < 0) delta = -delta;

    if (delta < 0.000001) return;
    this->setOpacity(current + (current > excepted ? -0.01 : 0.01));
}

void GraphicsButton::setText(const QString &text) {
    this->internalText->setText(text);
    this->updateText();
}

void GraphicsButton::setVisible(bool visible) {
    QGraphicsItem::setVisible(visible);
    this->internalText->setVisible(visible);
    if (visible) this->exceptedOpacity = 0.15;
}

void GraphicsButton::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
    if (showBg) {
        painter->setBrush(QBrush(QColor(0, 0, 0)));
        painter->drawRoundedRect(this->rect(), 15, 15);
    }
}
