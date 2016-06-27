#ifndef NETOBJECTITEM_H
#define NETOBJECTITEM_H

#include <QObject>
#include <QGraphicsPixmapItem>
#include <QList>

QT_BEGIN_NAMESPACE
class QPixmap;
class QGraphicsItem;
class QGraphicsScene;
class QTextEdit;
class QGraphicsSceneMouseEvent;
class QMenu;
class QGraphicsSceneContextMenuEvent;
class QPainter;
class QStyleOptionGraphicsItem;
class QWidget;
class QPolygonF;
class ProjectGraphicsObject;
QT_END_NAMESPACE

class ArrowItem;
class Place;
class TerminalTransition;
class NonTerminalTransition;

//! [0]
class NetObjectItem : public QObject, public QGraphicsPolygonItem
{
        Q_OBJECT
    public:
        enum { Type = UserType + 15 };
        enum ElementType
        {
            PlaceType = UserType,
            TerminalTransitionType = UserType + 1,
            NonTerminalTransitionType = UserType + 2
        };
        NetObjectItem(Place *aPlace);
        NetObjectItem(TerminalTransition *aTransition);
        NetObjectItem(NonTerminalTransition *aTransition);
        NetObjectItem(ElementType elementType, const QString& text, QGraphicsItem *parent = 0);
        ~NetObjectItem();

        void removeArrow(ArrowItem *arrow);
        void removeArrows();
        ElementType elementType() const { return mElementType; }
        QPolygonF polygon() const { return mPolygon; }
        quint64 ID();
        void addArrow(ArrowItem *arrow);
        int type() const Q_DECL_OVERRIDE { return Type;}

    protected:
        void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
        void contextMenuEvent(QGraphicsSceneContextMenuEvent *event) Q_DECL_OVERRIDE;
        QVariant itemChange(GraphicsItemChange change, const QVariant &value) Q_DECL_OVERRIDE;

    private:
        ElementType mElementType;
        QPolygonF mPolygon;
        QList<ArrowItem *> mArrows;
        QGraphicsTextItem *mTextItem;
        ProjectGraphicsObject *mGraphicsObject;

        void setMyPolygon();
        void setTextItem(const QString& text);
        void initialize(const QString& text, ElementType elementType);
        void mousePressEvent(QGraphicsSceneMouseEvent *event);
        void initialize(ProjectGraphicsObject *aGraphicsObject);

    signals:
        void selected(const quint64 &aID);
    private slots:
        void resourceNumberChanged(const quint64 &aResourceNumber);
        void sortChanged();
        void netChanged(const quint64 &aClassID, const quint64 &aNetID);
};
//! [0]

#endif // NETOBJECTITEM_H
