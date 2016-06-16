#ifndef PROJECTGRAPHICSOBJECT_H
#define PROJECTGRAPHICSOBJECT_H

#include "ProjectObject.h"

class ProjectGraphicsObject : public ProjectObject
{
        Q_OBJECT
    public:
        explicit ProjectGraphicsObject(const Type &aType, const quint64 &aID, const QPointF &aPoint = QPointF());

        void load(QXmlStreamReader *aInputStream);
        void save(QXmlStreamWriter *aOutputStream) const;

        QPointF pos() const;
        void setPos(const QPointF &aPoint);
    protected:
        ProjectGraphicsObject();

    private:
        QPointF mPoint;
    signals:

    public slots:
};

#endif // PROJECTGRAPHICSOBJECT_H
