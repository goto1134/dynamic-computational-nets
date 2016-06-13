#include "NetClass.h"
#include "ObjectNet.h"
#include "Place.h"
#include "ProjectModel.h"
#include <QDebug>

const QString NET_CLASS_LABEL = "net_class";
const QString OBJECT_NETS_LABEL = "object_nets";


NetClass::NetClass(const QString &aName, const quint64 &aID)
    :ProjectNamedObject(ProjectObject::NetClass, aName, aID)
{
}

NetClass::NetClass(QXmlStreamReader *aInputStream)
{
    load(aInputStream);
}

quint64 NetClass::createObjectNet(QString aNetName)
{
    foreach (ObjectNet *net, mObjectNets.values())
    {
        if(net->name() == aNetName)
        {
            return 0;
        }
    }
    quint64 id = ProjectModel::getInstance().generateID();
    mObjectNets.insert(id, new ObjectNet(aNetName, id, ID()));
    return id;
}

ObjectNet *NetClass::getObjectNetByID(const quint64 &aID)
{
    return mObjectNets.value(aID, 0);
}

QList<quint64> NetClass::getObjectNetsIDs() const
{
    return mObjectNets.keys();
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
        if(aInputStream->readNextStartElement()
                && aInputStream->name() == OBJECT_NETS_LABEL)
        {
            while (aInputStream->readNextStartElement()
                   && !(aInputStream->isEndElement()
                        && aInputStream->name() == OBJECT_NETS_LABEL)
                   )
            {
                ObjectNet *net = new ObjectNet(aInputStream);
                mObjectNets.insert(net->ID(), net);
            }
        }
    }
    qDebug() << aInputStream->name();
    aInputStream->readNextStartElement();
    qDebug() << aInputStream->name();

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
        aOutputStream->writeStartElement(OBJECT_NETS_LABEL);
        {
            foreach (ObjectNet *net, mObjectNets)
            {
                net->save(aOutputStream);
            }
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
