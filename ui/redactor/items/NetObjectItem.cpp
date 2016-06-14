#include "NetObjectItem.h"
#include "ArrowItem.h"

#include <QGraphicsScene>
#include <QGraphicsSceneContextMenuEvent>
#include <QMenu>
#include <QPainter>
#include "../../../model/Place.h"
#include "../../../model/ProjectModel.h"
#include "../../../model/ElementSort.h"


const QString DELETE_ACTION_NAME = "Delete";
const QString ADD_ONE_ACTION_NAME = "Add One";
const QString SUBTRACT_ONE_ACTION_NAME = "Subtract One";

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

NetObjectItem::NetObjectItem(Place *aPlace)
    :NetObjectItem(PlaceType, QString::number(aPlace->resourceNumber()))
{
    setPos(aPlace->pos());
    mGraphicsObject = aPlace;
    connect(aPlace, SIGNAL(sortChanged(quint64)), this, SLOT(sortChanged(quint64)));
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

void NetObjectItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
   QGraphicsPolygonItem::mousePressEvent(event);
   if(event->button() == Qt::LeftButton)
   {
       emit selected(mGraphicsObject->ID());
   }
}

void NetObjectItem::resourceNumberChanged(const quint64 &aResourceNumber)
{
    mTextItem->setPlainText(QString::number(aResourceNumber));
    update();
}

void NetObjectItem::sortChanged(const quint64 &aSortID)
{
    update();
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

void NetObjectItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    setPen(QPen(Qt::black, 8));
    setBrush(QBrush(Qt::white));
    if(mElementType == PlaceType)
    {
        Place *place = static_cast<Place *>(mGraphicsObject);
        QColor color = ProjectModel::getInstance().getSortByID(place->sortID())->color();
        QPen pen(color, 8);
        painter->setPen(pen);
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
        foreach (ArrowItem *arrow, mArrows)
        {
            arrow->updatePosition();
        }
    }
    return value;
}
