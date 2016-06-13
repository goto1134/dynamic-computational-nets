#include "ProjectObject.h"
#include <QFrame>
#include <QVBoxLayout>

const QString OBJECT_LABEL = "object";
const QString TYPE_LABEL = "type";

ProjectObject::ProjectObject()
{
}

ProjectObject::ProjectObject(const Type &aType)
{
    mType = aType;
}

ProjectObject::ProjectObject(const Type &aType, const quint64 &aID)
{
    mID = aID;
    mType = aType;
}

ProjectObject::ProjectObject(QXmlStreamReader *aInputStream)
{
    load(aInputStream);
}

ProjectObject::Type ProjectObject::type() const
{
    return mType;
}

quint64 ProjectObject::ID() const
{
    return mID;
}

void ProjectObject::setID(const quint64 &aID)
{
    mID = aID;
}

void ProjectObject::setType(const Type &aType)
{
    mType = aType;
}

void ProjectObject::load(QXmlStreamReader *aInputStream)
{
    if(aInputStream->isStartElement()
            && aInputStream->name() == OBJECT_LABEL)
    {
        foreach(QXmlStreamAttribute attribute,  aInputStream->attributes())
        {
            QString name = attribute.name().toString();
            if(name == ID_LABEL)
            {
                mID = attribute.value().toULongLong();
            }
            else if( name == TYPE_LABEL)
            {
                mType = static_cast<ProjectObject::Type>(attribute.value().toUInt());
            }
        }
    }
    aInputStream->skipCurrentElement();
}

QList<QWidget *> *ProjectObject::getPropertyWidgets()
{
    QList<QWidget *> *propertyWidgets = new QList<QWidget *>();
//    QFrame *widget = new QFrame();
//    QVBoxLayout *mainLayout = new QVBoxLayout();
//    widget->setLayout(mainLayout);
//    mainLayout->addItem();

//    propertyWidgets->append();
    return propertyWidgets;
}

void ProjectObject::save(QXmlStreamWriter *aOutputStream) const
{
    aOutputStream->writeStartElement(OBJECT_LABEL);
    {
        aOutputStream->writeAttribute(ID_LABEL, QString::number(mID));
        aOutputStream->writeAttribute(TYPE_LABEL, QString::number(mType));
    }
    aOutputStream->writeEndElement();
}


