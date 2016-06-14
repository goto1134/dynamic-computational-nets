#ifndef OBJECTNETREDACTOR_H
#define OBJECTNETREDACTOR_H

#include <QtGui>
#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>

class ConnectionItem;
class ObjectNet;

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
        bool mIsDrawing;
    signals:

    public slots:
        void setMouseTool();
        void setPlaceTool();
        void setTerminalTransitionTool();
        void setNonTerminalTransitionTool();
        void setConnectionTool();
    private slots:
};

#endif // OBJECTNETREDACTOR_H
