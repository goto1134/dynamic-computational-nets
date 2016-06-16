#include "ProjectNamedObject.h"

const QString NAMED_OBJECT_LABEL = "named_object";
const QString NAME_LABEL = "name";

ProjectNamedObject::ProjectNamedObject()
    :ProjectObject()
{

}

ProjectNamedObject::ProjectNamedObject(const Type &aType, const QString &aName, const quint64 &aID)
    :ProjectObject(aType, aID)
{
    mName = aName;
}

ProjectNamedObject::ProjectNamedObject(QXmlStreamReader *aInputStream)
{
    load(aInputStream);
}

void ProjectNamedObject::load(QXmlStreamReader *aInputStream)
{
    if (aInputStream->isStartElement()
            && aInputStream->name() == NAMED_OBJECT_LABEL)
    {
        foreach (QXmlStreamAttribute attribute, aInputStream->attributes())
        {
            if(attribute.name() == NAME_LABEL)
            {
                mName = attribute.value().toString();
            }
        }
    }
    aInputStream->readNextStartElement();
    ProjectObject::load(aInputStream);
    aInputStream->skipCurrentElement();
}

void ProjectNamedObject::save(QXmlStreamWriter *aOutputStream) const
{
    aOutputStream->writeStartElement(NAMED_OBJECT_LABEL);
    {
        aOutputStream->writeAttribute(NAME_LABEL, mName);
        ProjectObject::save(aOutputStream);
    }
    aOutputStream->writeEndElement();
}

QString ProjectNamedObject::name() const
{
    return mName;
}

void ProjectNamedObject::setName(const QString &aName)
{
    mName = aName;
    emit nameChanged(aName);
}



