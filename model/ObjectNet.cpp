#include "ObjectNet.h"
#include "Place.h"
#include "NonTerminalTransition.h"
#include "Connection.h"
#include "ProjectModel.h"
#include "NetClass.h"

const QString OBJECT_NET = "object_net";
const QString TRANSITIONS_LABEL = "transitions";
const QString CLASS_ID = "net_class_id";

ObjectNet::ObjectNet(const QString &aName, const quint64 &aID, const quint64 &aNetClassID)
    :ProjectNamedObject(ProjectObject::Net, aName, aID)
{
    mNetClassID = aNetClassID;
}

ObjectNet::ObjectNet(QXmlStreamReader *aInputStream)
{
    load(aInputStream);
}

void ObjectNet::load(QXmlStreamReader *aInputStream)
{
    if (aInputStream->isStartElement()
            && aInputStream->name() == OBJECT_NET)
    {
        foreach (QXmlStreamAttribute attribute, aInputStream->attributes())
        {
            if(attribute.name() == CLASS_ID)
            {
                mNetClassID = attribute.value().toULongLong();
            }
        }
        if(aInputStream->readNextStartElement()
                && aInputStream->name() == PLACES_LABEL
                && !aInputStream->isEndElement())
        {
            do
            {
                if(aInputStream->readNextStartElement()
                        && aInputStream->name() == PLACE_LABEL)
                {
                    Place *place = new Place(aInputStream);
                    mPlaces.insert(place->ID(), place);
                }
            }
            while(!(aInputStream->isEndElement()
                    && aInputStream->name() == PLACES_LABEL));
        }
        if(aInputStream->readNextStartElement()
                && aInputStream->name() == CONNECTIONS_LABEL
                && !aInputStream->isEndElement())
        {
            do
            {
                if(aInputStream->readNextStartElement()
                        && aInputStream->name() == CONNECTION_LABEL)
                {
                    Connection *connection = new Connection(aInputStream, this);
                    mConnections.insert(connection->ID(), connection);
                }
            }
            while(!(aInputStream->isEndElement()
                    && aInputStream->name() == CONNECTIONS_LABEL));
        }
        if(aInputStream->readNextStartElement()
                && aInputStream->name() == TRANSITIONS_LABEL
                && !aInputStream->isEndElement())
        {
            do
            {
                if(aInputStream->readNextStartElement())
                {
                    if(aInputStream->name() == TERMINAL_TRANSITION)
                    {
                        TerminalTransition *transition = new TerminalTransition(aInputStream);
                        mTTransitions.insert(transition->ID(), transition);
                    }
                    else if(aInputStream->name() == NON_TERMINAL_TRANSITION)
                    {
                        NonTerminalTransition *transition = new NonTerminalTransition(aInputStream);
                        mNTTransitions.insert(transition->ID(), transition);
                    }
                }
            }
            while(!(aInputStream->isEndElement()
                    && aInputStream->name() == TRANSITIONS_LABEL));
        }
    }
    aInputStream->readNextStartElement();
    ProjectNamedObject::load(aInputStream);
    aInputStream->skipCurrentElement();
}

void ObjectNet::save(QXmlStreamWriter *aOutputStream) const
{
    aOutputStream->writeStartElement(OBJECT_NET);
    {
        aOutputStream->writeAttribute(CLASS_ID, QString::number(mNetClassID));
        aOutputStream->writeStartElement(PLACES_LABEL);
        {
            foreach (Place *place, mPlaces.values())
            {
                place->save(aOutputStream);
            }
        }
        aOutputStream->writeEndElement();
        aOutputStream->writeStartElement(CONNECTIONS_LABEL);
        {
            foreach (Connection *connection, mConnections.values())
            {
                connection->save(aOutputStream);
            }
        }
        aOutputStream->writeEndElement();
        aOutputStream->writeStartElement(TRANSITIONS_LABEL);
        {
            foreach (TerminalTransition *transition, mTTransitions.values())
            {
                transition->save(aOutputStream);
            }
            foreach (NonTerminalTransition *transition, mNTTransitions.values())
            {
                transition->save(aOutputStream);
            }
        }
        aOutputStream->writeEndElement();
        ProjectNamedObject::save(aOutputStream);
    }
    aOutputStream->writeEndElement();
}

void ObjectNet::netClassPlaceRemoved(const quint64 &aID)
{
    foreach (Connection *connection, mConnections.values())
    {
        if(connection->endID() == aID)
        {
            quint64 connectionID = connection->ID();
            quint64 startID = connection->startID();
            mConnections.remove(connectionID);
            mPlaces.value(startID)->removeOutputConnectionID(connectionID);
        }
        else if(connection->startID() == aID)
        {
            quint64 connectionID = connection->ID();
            quint64 endID = connection->endID();
            mConnections.remove(connectionID);
            mPlaces.value(endID)->removeInputConnectionID(connectionID);
        }
    }
}

quint64 ObjectNet::netClassID() const
{
    return mNetClassID;
}

QList<Place *> ObjectNet::places()
{
    QList<Place *> places = mPlaces.values();
    places.append(ProjectModel::getInstance().getNetClassByID(mNetClassID)->places());
    return places;
}

QList<TerminalTransition *> ObjectNet::transitions()
{
    QList<TerminalTransition *> transitions = mTTransitions.values();
    return transitions;
}

QList<NonTerminalTransition *> ObjectNet::nonTerminalTransitions()
{
    QList<NonTerminalTransition *> transitions = mNTTransitions.values();
    return transitions;
}

TerminalTransition *ObjectNet::getTransitionByID(const quint64 &aID)
{
    if(TerminalTransition *transition = mTTransitions.value(aID, 0))
    {
        return transition;
    }
    else
    {
        return mNTTransitions.value(aID,0);
    }
}

QList<Connection *> ObjectNet::connections()
{
    QList<Connection *> connections = mConnections.values();
    return connections;
}

Place *ObjectNet::getPlaceByID(const quint64 &aID)
{
    Place *place;
    if(!(place= mPlaces.value(aID, 0)))
    {
        place = ProjectModel::getInstance().getNetClassByID(mNetClassID)->getPlaceByID(aID);
    }
    return place;
}

Place *ObjectNet::addPlace(const QPointF &aPoint)
{
    Place *place = new Place(ProjectModel::getInstance().generateID(), aPoint);
    mPlaces.insert(place->ID(), place);
    return place;
}

TerminalTransition *ObjectNet::addTransition(const QPointF &aPoint)
{
    TerminalTransition *transition = new TerminalTransition(ProjectModel::getInstance().generateID(), aPoint);
    mTTransitions.insert(transition->ID(), transition);
    return transition;
}

NonTerminalTransition *ObjectNet::addNonTerminalTransition(const QPointF &aPoint)
{
    NonTerminalTransition *transition = new NonTerminalTransition(ProjectModel::getInstance().generateID(), aPoint);
    mNTTransitions.insert(transition->ID(), transition);
    return transition;
}

Connection *ObjectNet::addConnection(const quint64 &aStartID, const quint64 &aEndID, const Connection::ConnectionVariant &aType)
{
    Connection *connection = new Connection(ProjectModel::getInstance().generateID(),
                                            aStartID, aEndID, aType, this);
    if(aType == Connection::InputTerminal
            || aType == Connection::InputNonTerminal)
    {
        if(Place *place = getPlaceByID(aStartID))
        {
            if(aType == Connection::InputTerminal)
            {
                TerminalTransition *transition = mTTransitions.value(aEndID);
                place->addOutputConnectionID(connection->ID());
                transition->addInputConnectionID(connection->ID());
                mConnections.insert(connection->ID(), connection);
                return connection;
            }
            else if(aType == Connection::InputNonTerminal)
            {
                NonTerminalTransition *transition = mNTTransitions.value(aEndID);
                place->addOutputConnectionID(connection->ID());
                transition->addInputConnectionID(connection->ID());
                mConnections.insert(connection->ID(), connection);
                return connection;
            }
        }
    }
    else if(aType == Connection::OutputTerminal
            || aType == Connection::OutputNonTerminal)
    {
        if(Place *place = getPlaceByID(aEndID))
        {
            if(aType == Connection::OutputTerminal)
            {
                TerminalTransition *transition = mTTransitions.value(aStartID);
                place->addInputConnectionID(connection->ID());
                transition->addOutputConnectionID(connection->ID());
                mConnections.insert(connection->ID(), connection);
                return connection;
            }
            else if(aType == Connection::OutputNonTerminal)
            {
                NonTerminalTransition *transition = mNTTransitions.value(aStartID);
                place->addInputConnectionID(connection->ID());
                transition->addOutputConnectionID(connection->ID());
                mConnections.insert(connection->ID(), connection);
                return connection;
            }
        }
    }
    return 0;
}

Connection *ObjectNet::getConnectionByID(const quint64 &aID)
{
    return mConnections.value(aID, 0);
}

void ObjectNet::deletePlace(const quint64 &aID)
{

}
