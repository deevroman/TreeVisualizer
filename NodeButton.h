#include <QObject>
#include <QGraphicsItem>
#include <QPainter>
#include <QGraphicsRectItem>
#include <QRect>
#include "Trees.h"

#ifndef MYQTPROJECT_NODEBUTTON_H
#define MYQTPROJECT_NODEBUTTON_H

class NodeButton : public QObject, public QGraphicsRectItem {
    Q_OBJECT
public:
    explicit NodeButton(QObject *parent = nullptr){};
    explicit NodeButton(const QRectF & rect, QGraphicsItem * parent = nullptr): QGraphicsRectItem(rect, parent) {}
    ~NodeButton() = default;
    int nodePointer;
signals:
    void clickNode(int);

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event){
//        QGraphicsItem::mousePressEvent(event);
        emit clickNode(nodePointer);
    }

private:
    virtual QRectF boundingRect() const {
        return this->rect();
    }
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
        painter->setBrush(this->brush());
        painter->drawEllipse(this->rect());
        Q_UNUSED(option);
        Q_UNUSED(widget);
    }
};


#endif //MYQTPROJECT_NODEBUTTON_H
