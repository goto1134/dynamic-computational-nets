#ifndef DIAGRAMITEM_H
#define DIAGRAMITEM_H

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
QT_END_NAMESPACE

class ArrowItem;

//! [0]
class NetObjectItem : public QGraphicsPolygonItem
{
public:
    enum { Type = UserType + 15 };
    enum ElementType
    {
        Place = UserType,
        TerminalTransition = UserType + 1,
        NonTerminalTransition = UserType + 2
    };

    NetObjectItem(ElementType elementType, const QString& text, QMenu *contextMenu, QGraphicsItem *parent = 0);
    ~NetObjectItem();

    void removeArrow(ArrowItem *arrow);
    void removeArrows();
    ElementType elementType() const { return mElementType; }
    QPolygonF polygon() const { return mPolygon; }
    void addArrow(ArrowItem *arrow);
    QPixmap image() const;
    int type() const Q_DECL_OVERRIDE { return Type;}

    protected:
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    void contextMenuEvent(QGraphicsSceneContextMenuEvent *event) Q_DECL_OVERRIDE;
    QVariant itemChange(GraphicsItemChange change, const QVariant &value) Q_DECL_OVERRIDE;

private:
    ElementType mElementType;
    QPolygonF mPolygon;
    QMenu *mContextMenu;
    QList<ArrowItem *> mArrows;
    QGraphicsTextItem *mTextItem;

    void setMyPolygon();
    void setTextItem(const QString& text);
};
//! [0]

#endif // DIAGRAMITEM_H
