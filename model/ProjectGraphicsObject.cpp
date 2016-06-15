#include "ProjectGraphicsObject.h"
#include <QDebug>
#include <QPointF>

const QString GRAPHICS_OBJECT = "graphics_object";
const QString POS_X = "pos_x";
const QString POS_Y = "pos_y";

ProjectGraphicsObject::ProjectGraphicsObject(const Type &aType, const quint64 &aID, const QPointF &aPoint)
    :ProjectObject(aType, aID)
{
    mPoint = aPoint;
}

void ProjectGraphicsObject::load(QXmlStreamReader *aInputStream)
{
    if (aInputStream->isStartElement()
            && aInputStream->name() == GRAPHICS_OBJECT)
    {
        qreal x,y;
        foreach (QXmlStreamAttribute attribute, aInputStream->attributes())
        {
            if(attribute.name() == POS_X)
            {
                x = attribute.value().toDouble();
            }
            else if(attribute.name() == POS_Y)
            {
                y = attribute.value().toDouble();
            }
        }
        mPoint = QPoint(x, y);
    }
    aInputStream->readNextStartElement();
    ProjectObject::load(aInputStream);
    aInputStream->skipCurrentElement();
}

void ProjectGraphicsObject::save(QXmlStreamWriter *aOutputStream) const
{
    aOutputStream->writeStartElement(GRAPHICS_OBJECT);
    {
        aOutputStream->writeAttribute(POS_X, QString::number(mPoint.x()));
        aOutputStream->writeAttribute(POS_Y, QString::number(mPoint.y()));
        ProjectObject::save(aOutputStream);
    }
    aOutputStream->writeEndElement();
}

QPointF ProjectGraphicsObject::pos() const
{
    return mPoint;
}

void ProjectGraphicsObject::setPos(const QPointF &aPoint)
{
    mPoint = aPoint;
}

ProjectGraphicsObject::ProjectGraphicsObject()
{

}

