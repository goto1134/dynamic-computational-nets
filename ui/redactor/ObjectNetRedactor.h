#ifndef OBJECTNETREDACTOR_H
#define OBJECTNETREDACTOR_H

#include <QtGui>
#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QObject>

class ConnectionItem;
class ObjectNet;
class NetObjectItem;

class ObjectNetRedactor : public QGraphicsScene
{
        Q_OBJECT
    public:
        explicit ObjectNetRedactor(QObject *aParent = 0);
        enum RedactorTool
        {
            Mouse,
            PlaceTool,
            NonTerminalTransitionTool,
            TerminalTransitionTool,
            ConnectionTool
        };
        void setObjectNet(ObjectNet *aObjectNet);
        void setTool(RedactorTool aTool);

    protected:
        void mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent) Q_DECL_OVERRIDE;
        void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
        void mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent);
    private:
        void updatePlaces();
        ObjectNet* mObjectNet;
        QVariant mRedactorTool;
        QGraphicsLineItem *line;
        QMap<quint64, NetObjectItem *> mNetObjectItems;
        bool mIsDrawing;
        void updateTransitions();
        void addNetItem(NetObjectItem *aItem);
    signals:
        placeSelected(const quint64 &aClassID, const quint64 &aNetID, const quint64 &aObjectID);
        transitionSelected(const quint64 &aClassID, const quint64 &aNetID, const quint64 &aObjectID);
    public slots:
        void setMouseTool();
        void setPlaceTool();
        void setTerminalTransitionTool();
        void setNonTerminalTransitionTool();
        void setConnectionTool();
    private slots:
        void placeSelected(const quint64 &aObjectID);
        void transitionSelected(const quint64 &aObjectID);

};

#endif // OBJECTNETREDACTOR_H
