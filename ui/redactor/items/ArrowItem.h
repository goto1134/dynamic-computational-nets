#ifndef ARROWITEM_H
#define ARROWITEM_H

#include <QGraphicsLineItem>

#include "NetObjectItem.h"

QT_BEGIN_NAMESPACE
class QGraphicsPolygonItem;
class QGraphicsLineItem;
class QGraphicsScene;
class QRectF;
class QGraphicsSceneMouseEvent;
class QPainterPath;
class Connection;
QT_END_NAMESPACE

class ArrowItem : public QObject, public QGraphicsLineItem
{
        Q_OBJECT
    public:
        enum { Type = UserType + 4 };

        ArrowItem(NetObjectItem *startItem, NetObjectItem *endItem, Connection *aConnection, QGraphicsItem *parent = 0);
        ~ArrowItem();

        int type() const Q_DECL_OVERRIDE { return Type; }
        QRectF boundingRect() const Q_DECL_OVERRIDE;
        QPainterPath shape() const Q_DECL_OVERRIDE;
        void setColor(const QColor &color) { mColor = color; }
        NetObjectItem *startItem() const { return mStartItem; }
        NetObjectItem *endItem() const { return mEndItem; }

        void updatePosition();
    signals:
        void selected(const quint64 &aID);
    protected:
        void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0) Q_DECL_OVERRIDE;
        void contextMenuEvent(QGraphicsSceneContextMenuEvent *event);
        void mousePressEvent(QGraphicsSceneMouseEvent *event);
    private slots:
        void updateTextData();

    private:
        NetObjectItem *mStartItem;
        NetObjectItem *mEndItem;
        QColor mColor;
        QPolygonF arrowHead;
        QGraphicsTextItem *mTextItem;
        Connection *mConnection;
};

#endif // ARROWITEM_H
