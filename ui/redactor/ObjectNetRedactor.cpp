#include "ObjectNetRedactor.h"
#include <Qt>
#include <QtGui>
#include "items/NetObjectItem.h"
#include "items/ArrowItem.h"
#include "../../model/ObjectNet.h"
#include "../../model/Place.h"


ObjectNetRedactor::ObjectNetRedactor(QObject *aParent)
    : QGraphicsScene(aParent)
{
    mRedactorTool = Mouse;
    line = 0;
}

void ObjectNetRedactor::setObjectNet(ObjectNet *aObjectNet)
{
    this->clear();
    setMouseTool();
    mObjectNet = aObjectNet;
    updatePlaces();
    update();
}

void ObjectNetRedactor::setTool(ObjectNetRedactor::RedactorTool aTool)
{
    mRedactorTool = aTool;
}

void ObjectNetRedactor::setMouseTool()
{
    mRedactorTool = Mouse;
}

void ObjectNetRedactor::setPlaceTool()
{
    mRedactorTool = PlaceTool;
}

void ObjectNetRedactor::setTerminalTransitionTool()
{
    mRedactorTool = TerminalTransitionTool;
}

void ObjectNetRedactor::setNonTerminalTransitionTool()
{
    mRedactorTool = NonTerminalTransitionTool;
}

void ObjectNetRedactor::setConnectionTool()
{
    mRedactorTool = ConnectionTool;
}

void ObjectNetRedactor::placeSelected(const quint64 &aObjectID)
{
    emit placeSelected(mObjectNet->netClassID(), mObjectNet->ID(), aObjectID);
}

void ObjectNetRedactor::mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    //если нажали на левую кнопку мыши
    if(mouseEvent->button() == Qt::LeftButton)
    {
        if(mRedactorTool == PlaceTool)
        {
            NetObjectItem * item = new NetObjectItem(NetObjectItem::PlaceType,"0");
            item->setPos(mouseEvent->scenePos());
            addItem(item);
        }
        else if(mRedactorTool == TerminalTransitionTool)
        {
            NetObjectItem * item = new NetObjectItem(NetObjectItem::TerminalTransitionType,"");
            item->setPos(mouseEvent->scenePos());
            addItem(item);
        }
        else if(mRedactorTool == NonTerminalTransitionTool)
        {
            NetObjectItem * item = new NetObjectItem(NetObjectItem::NonTerminalTransitionType,"hui");
            item->setPos(mouseEvent->scenePos());
            addItem(item);
        }
        else if(mRedactorTool == ConnectionTool)
        {
            mIsDrawing = true;
            line = new QGraphicsLineItem(QLineF(mouseEvent->scenePos(),mouseEvent->scenePos()));
            line->setPen(QPen(Qt::black, 2));
            addItem(line);
        }
        else
        {
            QGraphicsScene::mousePressEvent(mouseEvent);
        }
    }
    else
    {
        QGraphicsScene::mousePressEvent(mouseEvent);
    }
    update();
}

void ObjectNetRedactor::mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    if (mRedactorTool == ConnectionTool
            && line != 0
            && mIsDrawing)
    {
        QLineF newLine(line->line().p1(), mouseEvent->scenePos());
        line->setLine(newLine);
        QGraphicsScene::mouseMoveEvent(mouseEvent);
    }
    else if (mRedactorTool == Mouse)
    {
        QGraphicsScene::mouseMoveEvent(mouseEvent);
    }
}

void ObjectNetRedactor::updatePlaces()
{
    foreach (Place *place, mObjectNet->places())
    {
        NetObjectItem *item = new NetObjectItem(place);
        addItem(item);
        connect(item, SIGNAL(selected(quint64)), this, SLOT(placeSelected(quint64)));
        update();
    }
}

void ObjectNetRedactor::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    if(mRedactorTool == ConnectionTool
            && line != 0
            && mIsDrawing)
    {
        mIsDrawing = false;
        QList<QGraphicsItem *> startItems = items(line->line().p1());
        if (startItems.count() && startItems.first() == line)
        {
            startItems.removeFirst();
        }
        QList<QGraphicsItem *> endItems = items(line->line().p2());
        if (endItems.count() && endItems.first() == line)
        {
            endItems.removeFirst();
        }

        removeItem(line);
        delete line;

        if(startItems.count() > 1)
        {
            foreach(QGraphicsItem *item, startItems)
            {
                if(item->type() != NetObjectItem::Type)
                {
                    startItems.removeOne(item);
                }
            }
        }
        if(endItems.count() > 1)
        {
            foreach(QGraphicsItem *item, endItems)
            {
                if(item->type() != NetObjectItem::Type)
                {
                    endItems.removeOne(item);
                }
            }
        }

        if (startItems.count() > 0 && endItems.count() > 0 &&
                startItems.first()->type() == NetObjectItem::Type &&
                endItems.first()->type() == NetObjectItem::Type &&
                startItems.first() != endItems.first())
        {
            NetObjectItem *startItem = qgraphicsitem_cast<NetObjectItem *>(startItems.first());
            NetObjectItem *endItem = qgraphicsitem_cast<NetObjectItem *>(endItems.first());
            if(
                    (startItem->elementType() == NetObjectItem::PlaceType
                     && ( endItem->elementType() == NetObjectItem::TerminalTransitionType
                          || endItem->elementType() == NetObjectItem::NonTerminalTransitionType )
                     )
                    || (
                        ( startItem->elementType() == NetObjectItem::TerminalTransitionType
                          || startItem->elementType() == NetObjectItem::NonTerminalTransitionType
                          )
                        && endItem->elementType() == NetObjectItem::PlaceType)
                    )
            {
                ArrowItem *arrow = new ArrowItem(startItem, endItem);
                arrow->setColor(Qt::black);
                startItem->addArrow(arrow);
                endItem->addArrow(arrow);
                arrow->setZValue(-1000.0);
                addItem(arrow);
                arrow->updatePosition();
            }
        }
    }
    else
    {
        QGraphicsScene::mouseReleaseEvent(event);
    }
    update();
}

