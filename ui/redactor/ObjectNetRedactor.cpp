#include "ObjectNetRedactor.h"
#include <Qt>
#include <QtGui>
#include "items/NetObjectItem.h"
#include "items/ArrowItem.h"

ObjectNetRedactor::ObjectNetRedactor(QObject *aParent)
    : QGraphicsScene(aParent)
{
    mRedactorTool = Mouse;
    line = 0;
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
    mRedactorTool = Place;
}

void ObjectNetRedactor::setTerminalTransitionTool()
{
    mRedactorTool = TerminalTransition;
}

void ObjectNetRedactor::setNonTerminalTransitionTool()
{
    mRedactorTool = NonTerminalTransition;
}

void ObjectNetRedactor::setConnectionTool()
{
    mRedactorTool = Connection;
}

void ObjectNetRedactor::mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    //если нажали на левую кнопку мыши
    if(mouseEvent->button() == Qt::LeftButton)
    {
        if(mRedactorTool == Place)
        {
            NetObjectItem * item = new NetObjectItem(NetObjectItem::Place,"0",0);
            item->setPos(mouseEvent->scenePos());
            addItem(item);
        }
        else if(mRedactorTool == TerminalTransition)
        {
            NetObjectItem * item = new NetObjectItem(NetObjectItem::TerminalTransition,"",0);
            item->setPos(mouseEvent->scenePos());
            addItem(item);
        }
        else if(mRedactorTool == NonTerminalTransition)
        {
            NetObjectItem * item = new NetObjectItem(NetObjectItem::NonTerminalTransition,"hui",0);
            item->setPos(mouseEvent->scenePos());
            addItem(item);
        }
        else if(mRedactorTool == Connection)
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
    if (mRedactorTool == Connection
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

void ObjectNetRedactor::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsItem *target;
    if(mRedactorTool == Connection
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
            if(startItem->elementType() == NetObjectItem::Place
                    && ( endItem->elementType() == NetObjectItem::TerminalTransition
                        || endItem->elementType() == NetObjectItem::NonTerminalTransition )
                    || ( startItem->elementType() == NetObjectItem::TerminalTransition
                        || startItem->elementType() == NetObjectItem::NonTerminalTransition )
                    && endItem->elementType() == NetObjectItem::Place)
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

