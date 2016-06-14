#include "NetObjectItem.h"
#include "ArrowItem.h"

#include <QGraphicsScene>
#include <QGraphicsSceneContextMenuEvent>
#include <QMenu>
#include <QPainter>
#include "../../../model/Place.h"

const QString DELETE_ACTION_NAME = "Delete";
const QString ADD_ONE_ACTION_NAME = "Add One";
const QString SUBTRACT_ONE_ACTION_NAME = "Subtract One";


void NetObjectItem::setMyPolygon()
{
    switch (mElementType) {
        case PlaceType:
            mPolygon << QPointF(-25, -25) << QPointF(25, -25)
                     << QPointF(25, 25) << QPointF(-25, 25)
                     << QPointF(-25, -25);
            break;
        case TerminalTransitionType:
            mPolygon << QPointF(-10, -100) << QPointF(10, -100)
                     << QPointF(10, 100) << QPointF(-10, 100)
                     << QPointF(-10, -100);
            break;
        case NonTerminalTransitionType:
            mPolygon << QPointF(-30, -100) << QPointF(30, -100)
                     << QPointF(30, 100) << QPointF(-30, 100)
                     << QPointF(-30, -100);
            break;
    }
    setPolygon(mPolygon);
}

void NetObjectItem::setTextItem(const QString& text)
{
    mTextItem = new QGraphicsTextItem(text, this);
    if(mElementType == PlaceType)
    {
        mTextItem->setPos(pos().x()-10, pos().y()-10);
    }
    else if(mElementType == NonTerminalTransitionType)
    {
        mTextItem->setPos(pos().x()-9, pos().y()-90);
        mTextItem->setRotation(90);
    }
}

NetObjectItem::NetObjectItem(Place *aPlace)
    :NetObjectItem(PlaceType, QString::number(aPlace->resourceNumber()))
{
    setPos(aPlace->pos());
}

NetObjectItem::NetObjectItem(ElementType elementType, const QString& text, QGraphicsItem *parent)
    : QGraphicsPolygonItem(parent)
{
    mElementType = elementType;

    QPainterPath path;

    setTextItem(text);

    setMyPolygon();
    setFlag(QGraphicsItem::ItemIsMovable, true);
    setFlag(QGraphicsItem::ItemIsSelectable, true);
    setFlag(QGraphicsItem::ItemSendsGeometryChanges, true);
}

NetObjectItem::~NetObjectItem()
{
    delete mTextItem;
    foreach(ArrowItem *arrow, mArrows)
    {
        delete arrow;
    }
}

void NetObjectItem::removeArrow(ArrowItem *arrow)
{
    int index = mArrows.indexOf(arrow);

    if (index != -1)
    {
        mArrows.removeAt(index);
    }
}

void NetObjectItem::removeArrows()
{
    foreach (ArrowItem *arrow, mArrows)
    {
        arrow->startItem()->removeArrow(arrow);
        arrow->endItem()->removeArrow(arrow);
        scene()->removeItem(arrow);
        delete arrow;
    }
}

void NetObjectItem::addArrow(ArrowItem *arrow)
{
    mArrows.append(arrow);
}

QPixmap NetObjectItem::image() const
{
    QPixmap pixmap(250, 250);
    pixmap.fill(Qt::transparent);
    QPainter painter(&pixmap);
    painter.setPen(QPen(Qt::black, 8));
    painter.translate(125, 125);
    if(mElementType == PlaceType)
    {
        painter.drawEllipse(-125,-125,250,250);
    }
    else
    {
        painter.drawPolyline(mPolygon);
    }
    return pixmap;
}

void NetObjectItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    setPen(QPen(Qt::black, 8));
    setBrush(QBrush(Qt::white));
    if(mElementType == PlaceType)
    {
        painter->setPen(QPen(Qt::black, 8));
        painter->setBrush(QBrush(Qt::white));
        painter->drawEllipse(mapFromItem(this, 0, 0), 25, 25);
    }
    else
    {
        if(mElementType == TerminalTransitionType)
        {
            setBrush(QBrush(Qt::black));
        }
        QGraphicsPolygonItem::paint(painter, option, widget);
    }
}

void NetObjectItem::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{
    QMenu menu;
    menu.addAction(DELETE_ACTION_NAME);
    if(mElementType == PlaceType)
    {
        menu.addAction(ADD_ONE_ACTION_NAME);
        menu.addAction(SUBTRACT_ONE_ACTION_NAME);
    }
    QAction *a = menu.exec(event->screenPos());
    if(a)
    {
        if(a->text() == DELETE_ACTION_NAME)
        {
            delete this;
        }
        else if (a->text() == ADD_ONE_ACTION_NAME)
        {
            mTextItem->setPlainText(QString::number(mTextItem->toPlainText().toInt() + 1));
        }
        else if (a->text() == SUBTRACT_ONE_ACTION_NAME)
        {
            mTextItem->setPlainText(QString::number(mTextItem->toPlainText().toInt() - 1));
        }
    }
}

QVariant NetObjectItem::itemChange(GraphicsItemChange change, const QVariant &value)
{
    if (change == QGraphicsItem::ItemPositionChange) {
        foreach (ArrowItem *arrow, mArrows) {
            arrow->updatePosition();
        }
    }
    return value;
}
