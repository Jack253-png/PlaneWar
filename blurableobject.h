#ifndef BLURABLEOBJECT_H
#define BLURABLEOBJECT_H

#include <QObject>
#include <QGraphicsPixmapItem>
#include <QTimer>

class BlurableObject : public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT
public:
    explicit BlurableObject(QObject *parent = nullptr);
    bool show;
    int exceptedBlurRadius;
private:
    QTimer *timer;
    QTimer *blurTimer;

    int currentBlurRadius;
signals:

public slots:
    void onShowStateTimerTimeout();
    void onBlurTimerTimeout();
};

#endif // BLURABLEOBJECT_H
