#ifndef GRAPHICSBUTTON_H
#define GRAPHICSBUTTON_H

#include "vars.h"
#include <QObject>
#include <QGraphicsScene>
#include <QGraphicsSimpleTextItem>
#include <QGraphicsRectItem>
#include <QSize>
#include <QTimer>
#include <QGraphicsSceneHoverEvent>
#include <QGraphicsSceneMouseEvent>
#include <QPainter>
#include <QStyleOptionGraphicsItem>
#include <QWidget>

class GraphicsButton : public QObject, public QGraphicsRectItem {
    Q_OBJECT
public:
    explicit GraphicsButton(QObject *parent = nullptr);
//    GraphicsButton(QGraphicsScene *basescene = nullptr);
    void setPos(int x, int y, int w, int h);
    void setText(const QString &text);
    void updateText();
    void setVisible(bool visible);
    qreal exceptedOpacity;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

    bool showBg = true;
protected:
    virtual void mousePressEvent(QGraphicsSceneMouseEvent *event);
    virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
    virtual void hoverEnterEvent(QGraphicsSceneHoverEvent *event);
    virtual void hoverLeaveEvent(QGraphicsSceneHoverEvent *event);
private:
    QGraphicsScene *basescene;
    QGraphicsSimpleTextItem *internalText;
    QSize* calcPos();
    QTimer *timer;
signals:
    void onClicked();
public slots:
    void onOpacityUpdate();
};

#endif // GRAPHICSBUTTON_H
