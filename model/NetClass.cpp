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

QList<ObjectNet *> NetClass::getObjectNets()
{
    return mObjectNets.values();
}

QList<quint64> NetClass::getObjectNetsIDs() const
{
    return mObjectNets.keys();
}

void NetClass::getPlaces(QString aTagName, QXmlStreamReader *aInputStream, QMap<quint64, Place *> *aPlacesMap)
{
    if(aInputStream->readNextStartElement()
            && aInputStream->name() == aTagName)
    {
        do
        {
            if(aInputStream->readNextStartElement()
                    && aInputStream->name() == PLACE_LABEL)
            {
                Place *place = new Place(aInputStream);
                aPlacesMap->insert(place->ID(), place);
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

bool NetClass::setNetName(const quint64 &aNetID, const QString &aNewName)
{
    ProjectNamedObject *netWithID;
    foreach (ProjectNamedObject *objectNet, mObjectNets.values())
    {
        if(objectNet->name() == aNewName
                && objectNet->ID()!= aNetID)
        {
            return false;
        }
        if(objectNet->ID() == aNetID)
        {
            netWithID = objectNet;
        }
    }
    if(netWithID)
    {
        netWithID->setName(aNewName);
        return true;
    }
    else
    {
        return false;
    }
}

QList<Place *> NetClass::places()
{
    QList<Place *> places = mInputPlaces.values();
    places.append(mOutputPlaces.values());
    return places;
}

Place *NetClass::getPlaceByID(const quint64 &aPlaceID, const quint64 &aNetID)
{
    if(aNetID != 0)
    {
        if(Place *place = mObjectNets.value(aNetID)->getPlaceByID(aPlaceID))
        {
            return place;
        }
    }
    else
    {
        if(Place *place = mInputPlaces.value(aPlaceID, 0))
        {
            return place;
        }
        else
        {
            return mOutputPlaces.value(aPlaceID, 0);
        }
        foreach (ObjectNet *net, mObjectNets)
        {
            if(Place *place = net->getPlaceByID(aPlaceID))
            {
                return place;
            }
        }
    }
    return 0;
}

int NetClass::getInputPlaceNumber()
{
    return mInputPlaces.count();
}

int NetClass::getOutputPlaceNumber()
{
    return mOutputPlaces.count();
}

void NetClass::setInputPlaceNumber(const int &aInputNumber)
{
    setPlaceNumber(aInputNumber, true, &mInputPlaces);
}

void NetClass::setOutputPlaceNumber(const int &aOutputNumber)
{
    setPlaceNumber(aOutputNumber, false, &mOutputPlaces);
}

void NetClass::setPlaceNumber(const int &aInputNumber, const bool &aInput, QMap<quint64, Place *> *aPlacesMap)
{
    if(aInputNumber > aPlacesMap->size())
    {
        while (aInputNumber > aPlacesMap->size())
        {
            Place *place = new Place(ProjectModel::getInstance().generateID(),
                                     QPointF(aInput ? -500 : 500, aPlacesMap->size() * 100),
                                     aInput ? Place::Input : Place::Output);
            aPlacesMap->insert(place->ID(), place);
        }
    }
    else if (aPlacesMap->size() - aInputNumber > 0)
    {
        QList<quint64> keyList = aPlacesMap->keys();
        qSort(keyList);
        while(aInputNumber != aPlacesMap->size())
        {
            removePlace(keyList.last(), aPlacesMap);
            keyList.removeLast();
        }
    }
}

void NetClass::removePlace(quint64 &aID, QMap<quint64, Place *> *aPlacesMap)
{
    foreach (ObjectNet *net, mObjectNets.values())
    {
        net->netClassPlaceRemoved(aID);
    }
    aPlacesMap->remove(aID);
}

void NetClass::writePlaces(QXmlStreamWriter *aOutputStream, QMap<quint64, Place *> aPlacesMap, QString aTagName) const
{
    aOutputStream->writeStartElement(aTagName);
    foreach(Place *place, aPlacesMap.values())
    {
        place->save(aOutputStream);
    }
    aOutputStream->writeEndElement();
}
