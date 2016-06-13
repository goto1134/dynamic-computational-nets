#ifndef ARROW_H
#define ARROW_H

#include <QGraphicsLineItem>

#include "NetObjectItem.h"

QT_BEGIN_NAMESPACE
class QGraphicsPolygonItem;
class QGraphicsLineItem;
class QGraphicsScene;
class QRectF;
class QGraphicsSceneMouseEvent;
class QPainterPath;
QT_END_NAMESPACE

//! [0]
class ArrowItem : public QGraphicsLineItem
{
    public:
        enum { Type = UserType + 4 };

        ArrowItem(NetObjectItem *startItem, NetObjectItem *endItem, QGraphicsItem *parent = 0);
        ~ArrowItem();

        int type() const Q_DECL_OVERRIDE { return Type; }
        QRectF boundingRect() const Q_DECL_OVERRIDE;
        QPainterPath shape() const Q_DECL_OVERRIDE;
        void setColor(const QColor &color) { myColor = color; }
        NetObjectItem *startItem() const { return myStartItem; }
        NetObjectItem *endItem() const { return myEndItem; }

        void updatePosition();

    protected:
        void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0) Q_DECL_OVERRIDE;
        void contextMenuEvent(QGraphicsSceneContextMenuEvent *event);
    private:
        NetObjectItem *myStartItem;
        NetObjectItem *myEndItem;
        QColor myColor;
        QPolygonF arrowHead;
};
//! [0]

#endif // ARROW_H
