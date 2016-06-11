#ifndef OBJECTNETREDACTOR_H
#define OBJECTNETREDACTOR_H

#include <QtGui>
#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>

class ConnectionItem;

class ObjectNetRedactor : public QGraphicsScene
{
        Q_OBJECT

    public:
        explicit ObjectNetRedactor(QObject *aParent = 0);
        enum RedactorTool
        {
            Mouse,
            Place,
            NonTerminalTransition,
            TerminalTransition,
            Connection
        };

    protected:
        void mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent) Q_DECL_OVERRIDE;
        void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
        void mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent);
    private:
        QVariant mRedactorTool;
        QGraphicsLineItem *line;
        bool mIsDrawing;
    signals:

    public slots:
        void setTool(RedactorTool aTool);
        void setMouseTool();
        void setPlaceTool();
        void setTerminalTransitionTool();
        void setNonTerminalTransitionTool();
        void setConnectionTool();
    private slots:
};

#endif // OBJECTNETREDACTOR_H
