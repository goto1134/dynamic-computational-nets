#include "NetClass.h"
#include "ObjectNet.h"
#include "Place.h"

const QString NET_CLASS_LABEL = "net_class";

NetClass::NetClass(const QString &aName, const quint64 &aID)
    :ProjectNamedObject(ProjectObject::NetClass, aName, aID)
{
}

NetClass::NetClass(QXmlStreamReader *aInputStream)
{
    load(aInputStream);
}

bool NetClass::createObjectNet(QString aNetName)
{
    if(!mObjectNets.contains(aNetName))
    {
        mObjectNets.insert(aNetName, new ObjectNet(aNetName));
        return true;
    }
    return false;
}

void NetClass::getPlaces(QString aTagName, QXmlStreamReader *aInputStream, QSet<Place *> *aPlacesSet)
{
    if(aInputStream->readNextStartElement()
            && aInputStream->name() == aTagName)
    {
        do
        {
            if(aInputStream->readNextStartElement()
                    && aInputStream->name() == PLACE_LABEL)
            {
                aPlacesSet->insert(new Place(aInputStream));
            }
        }
        while(!(aInputStream->isEndElement()
                && aInputStream->name() == aTagName));

    }
}

void NetClass::load(QXmlStreamReader *aInputStream)
{
    if (aInputStream->isStartElement()
            && aInputStream->name() == NET_CLASS_LABEL)
    {
        if(aInputStream->readNextStartElement()
                && aInputStream->name() == PLACES_LABEL)
        {
            getPlaces(INPUT_LABEL, aInputStream, &mInputPlaces);
            getPlaces(OUTPUT_LABEL, aInputStream, &mOutputPlaces);

            aInputStream->skipCurrentElement();
        }
    }
    aInputStream->readNextStartElement();
    ProjectNamedObject::load(aInputStream);
    aInputStream->skipCurrentElement();
}

void NetClass::save(QXmlStreamWriter *aOutputStream) const
{
    aOutputStream->writeStartElement(NET_CLASS_LABEL);
    {
        aOutputStream->writeStartElement(PLACES_LABEL);
        {
            writePlaces(aOutputStream, mInputPlaces, INPUT_LABEL);
            writePlaces(aOutputStream, mOutputPlaces, OUTPUT_LABEL);
        }
        aOutputStream->writeEndElement();
    }
    ProjectNamedObject::save(aOutputStream);
    aOutputStream->writeEndElement();
}

void NetClass::addInputPlace(Place *aPlace)
{
    mInputPlaces.insert(aPlace);
}

void NetClass::addOutputPlace(Place *aPlace)
{
    mOutputPlaces.insert(aPlace);
}

void NetClass::writePlaces(QXmlStreamWriter *aOutputStream, QSet<Place *> aPlacesSet, QString aTagName) const
{
    aOutputStream->writeStartElement(aTagName);
    foreach(Place *place, aPlacesSet)
    {
        place->save(aOutputStream);
    }
    aOutputStream->writeEndElement();
}
