#include "Place.h"

const QString SORT_ID = "sort";
const QString PLACE_TYPE = "place_type";

Place::Place(const quint64 &aID, const QPointF &aPoint, const Place::PlaceVariant &type)
    :ProjectGraphicsObject(ProjectObject::PlaceType, aID, aPoint)
{
    mResourceNumber = type == Input ? 1 : 0;
    mSortID = 0;
    mPlaceType = type;
}

Place::Place(QXmlStreamReader *aInputStream)
{
    load(aInputStream);
}

void Place::getConnectionsIDs(QSet<quint64> *aConnectionSet, QXmlStreamReader *aInputStream, QString name)
{
    if(aInputStream->readNextStartElement()
            && aInputStream->name() == name)
    {
        do
        {
            if(aInputStream->readNextStartElement()
                    && aInputStream->name() == ID_LABEL)
            {
                aInputStream->readNext();
                aConnectionSet->insert(aInputStream->text().toULongLong());
                aInputStream->skipCurrentElement();
            }
        }
        while(!(aInputStream->isEndElement()
                && aInputStream->name() == name));
    }
}

void Place::load(QXmlStreamReader *aInputStream)
{
    if(aInputStream->isStartElement()
            && aInputStream->name() == PLACE_LABEL)
    {
        foreach(QXmlStreamAttribute attribute,  aInputStream->attributes())
        {
            QString name = attribute.name().toString();
            if(name == SORT_ID)
            {
                mSortID = attribute.value().toULongLong();
            }
            else if( name == RESOURCE_NUMBER_LABEL)
            {
                mResourceNumber = attribute.value().toULongLong();
            }
            else if(name == PLACE_TYPE)
            {
                mPlaceType = static_cast<Place::PlaceVariant>(attribute.value().toUInt());
            }
        }
        if(aInputStream->readNextStartElement()
                && aInputStream->name() == CONNECTIONS_LABEL)
        {
            getConnectionsIDs(&mInputConnIDs, aInputStream, INPUT_LABEL);
            getConnectionsIDs(&mOutputConnIDs, aInputStream, OUTPUT_LABEL);
            aInputStream->skipCurrentElement();
        }
        aInputStream->readNextStartElement();

        ProjectGraphicsObject::load(aInputStream);
    }
    aInputStream->skipCurrentElement();
}

void Place::save(QXmlStreamWriter *aOutputStream) const
{
    aOutputStream->writeStartElement(PLACE_LABEL);
    {
        aOutputStream->writeAttribute(SORT_ID, QString::number(mSortID));
        aOutputStream->writeAttribute(RESOURCE_NUMBER_LABEL, QString::number(mResourceNumber));
        aOutputStream->writeAttribute(PLACE_TYPE, QString::number(mPlaceType));

        aOutputStream->writeStartElement(CONNECTIONS_LABEL);
        {
            saveConnectionIDs(mInputConnIDs, aOutputStream, INPUT_LABEL);
            saveConnectionIDs(mOutputConnIDs, aOutputStream, OUTPUT_LABEL);
        }
        aOutputStream->writeEndElement();
        ProjectGraphicsObject::save(aOutputStream);
    }
    aOutputStream->writeEndElement();
}

/**
 * @brief Place::saveConnectionIDs сохраняет ID входящих и исходящих соединений.
 * @param aSource - QSet id
 * @param aOutputStream
 * @param aName - имя тега для сохранения
 */
void Place::saveConnectionIDs(QSet<quint64> aSource, QXmlStreamWriter *aOutputStream, QString aName) const
{
    aOutputStream->writeStartElement(aName);
    {
        foreach(quint64 id, aSource)
        {
            aOutputStream->writeTextElement(ID_LABEL, QString::number(id));
        }
    }
    aOutputStream->writeEndElement();
}

quint64 Place::sortID() const
{
    return mSortID;
}

void Place::setSortID(const quint64 &sortID)
{
    mSortID = sortID;
    emit sortChanged();
}

quint64 Place::resourceNumber() const
{
    return mResourceNumber;
}

void Place::setResourceNumber(const quint64 &resourceNumber)
{
    mResourceNumber = resourceNumber;
    emit resourceNumberChanged(resourceNumber);
}

void Place::addInputConnectionID(const quint64 &connectionID)
{
    mInputConnIDs.insert(connectionID);
}

void Place::addOutputConnectionID(const quint64 &connectionID)
{
    mOutputConnIDs.insert(connectionID);
}

void Place::removeInputConnectionID(const quint64 &connectionID)
{
    mInputConnIDs.remove(connectionID);
}

void Place::removeOutputConnectionID(const quint64 &connectionID)
{
    mOutputConnIDs.remove(connectionID);

}

Place::PlaceVariant Place::getPlaceType() const
{
    return mPlaceType;
}

void Place::setPlaceType(const PlaceVariant &placeType)
{
    mPlaceType = placeType;
}
