#include "ArrowItem.h"

#include <math.h>

#include <QPen>
#include <QPainter>
#include <QGraphicsSceneContextMenuEvent>

#include <model/Connection.h>

const qreal Pi = 3.14;

void ArrowItem::updateTextData()
{
    mTextItem->setPlainText("r = " + QString::number(mConnection->resources())
                            + ", t = " + QString::number(mConnection->time()));
}

ArrowItem::ArrowItem(NetObjectItem *startItem, NetObjectItem *endItem, Connection *aConnection, QGraphicsItem *parent)
    : QGraphicsLineItem(parent)
{
    mStartItem = startItem;
    mEndItem = endItem;
    setFlag(QGraphicsItem::ItemIsSelectable, true);
    mColor = Qt::black;
    setPen(QPen(mColor, 2, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    mConnection = aConnection;
    mTextItem = new QGraphicsTextItem(this);
    updateTextData();
    QFont font = mTextItem->font();
    font.setPointSize(14);
    mTextItem->setFont(font);
    connect(mConnection, SIGNAL(dataChanged()), this, SLOT(updateTextData()));
}

ArrowItem::~ArrowItem()
{
    mStartItem->removeArrow(this);
    mEndItem->removeArrow(this);
}

QRectF ArrowItem::boundingRect() const
{
    qreal extra = (pen().width() + 20) / 2.0;

    return QRectF(line().p1(), QSizeF(line().p2().x() - line().p1().x(),
                                      line().p2().y() - line().p1().y()))
            .normalized()
            .adjusted(-extra, -extra, extra, extra);
}

QPainterPath ArrowItem::shape() const
{
    QPainterPath path = QGraphicsLineItem::shape();
    path.addPolygon(arrowHead);
    return path;
}

void ArrowItem::updatePosition()
{
    QLineF line(mapFromItem(mStartItem, 0, 0), mapFromItem(mEndItem, 0, 0));
    setLine(line);
}

void ArrowItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *,QWidget *)
{
    if (mStartItem->collidesWithItem(mEndItem))
    {
        return;
    }

    QPen myPen = pen();
    myPen.setColor(mColor);
    qreal arrowSize = 20;
    painter->setPen(myPen);
    painter->setBrush(mColor);

    QLineF centerLine(mStartItem->pos(), mEndItem->pos());
    QPolygonF endPolygon = mEndItem->polygon();
    QPointF p1 = endPolygon.first() + mEndItem->pos();
    QPointF p2;
    QPointF intersectPoint;
    QLineF polyLine;
    for (int i = 1; i < endPolygon.count(); ++i)
    {
        p2 = endPolygon.at(i) + mEndItem->pos();
        polyLine = QLineF(p1, p2);
        QLineF::IntersectType intersectType = polyLine.intersect(centerLine, &intersectPoint);
        if (intersectType == QLineF::BoundedIntersection)
        {
            break;
        }
        p1 = p2;
    }

    setLine(QLineF(intersectPoint, mStartItem->pos()));


    double angle = acos(line().dx() / line().length());
    if (line().dy() >= 0)
    {
        angle = (Pi * 2) - angle;
    }

    QPointF arrowP1 = line().p1() + QPointF(sin(angle + Pi / 3) * arrowSize,
                                            cos(angle + Pi / 3) * arrowSize);
    QPointF arrowP2 = line().p1() + QPointF(sin(angle + Pi - Pi / 3) * arrowSize,
                                            cos(angle + Pi - Pi / 3) * arrowSize);

    arrowHead.clear();
    arrowHead << line().p1() << arrowP1 << arrowP2;

    painter->drawLine(line());
    painter->drawPolygon(arrowHead);
    if (isSelected())
    {
        painter->setPen(QPen(mColor, 1, Qt::DashLine));
        QLineF myLine = line();
        myLine.translate(0, 4.0);
        painter->drawLine(myLine);
        myLine.translate(0,-8.0);
        painter->drawLine(myLine);
    }
    mTextItem->setPos(mStartItem->pos() - (mStartItem->pos() - mEndItem->pos()) / 2 );
}

void ArrowItem::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{

}

void ArrowItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsLineItem::mousePressEvent(event);
    if(event->button() == Qt::LeftButton)
    {
        emit selected(mConnection->ID());
    }
}
