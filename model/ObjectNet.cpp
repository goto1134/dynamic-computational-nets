#include "ObjectNet.h"
#include "Place.h"
#include "NonTerminalTransition.h"
#include "Connection.h"

const QString OBJECT_NET = "object_net";
const QString TRANSITIONS_LABEL = "transitions";
const QString CLASS_ID = "net_class_id";

ObjectNet::ObjectNet(const QString &aName)
    :ProjectNamedObject(ProjectObject::Net, aName)
{

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
            bool found = false;
            while (aInputStream->readNextStartElement()
                   && aInputStream->name() == PLACE_LABEL)
            {
                found = true;
                mPlaces.insert(new Place(aInputStream));
            }
            if(found)
            {
                aInputStream->skipCurrentElement();
            }
        }
        if(aInputStream->readNextStartElement()
                && aInputStream->name() == CONNECTIONS_LABEL
                && !aInputStream->isEndElement())
        {
            bool found = false;
            while (aInputStream->readNextStartElement()
                   && aInputStream->name() == CONNECTION_LABEL)
            {
                mConnections.insert(new Connection(aInputStream));
            }
            if(found)
            {
                aInputStream->skipCurrentElement();
            }
        }
        if(aInputStream->readNextStartElement()
                && aInputStream->name() == TRANSITIONS_LABEL
                && !aInputStream->isEndElement())
        {
            bool found = false;
            while (aInputStream->readNextStartElement())
            {
                if(aInputStream->name() == TERMINAL_TRANSITION)
                {
                    found = true;
                    mTTransitions.insert(new TerminalTransition(aInputStream));
                }
                else if(aInputStream->name() == NON_TERMINAL_TRANSITION)
                {
                    found = true;
                    mNTTransitions.insert(new NonTerminalTransition(aInputStream));
                }
            }
            if(found)
            {
                aInputStream->skipCurrentElement();
            }
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
            foreach (Place *place, mPlaces)
            {
                place->save(aOutputStream);
            }
        }
        aOutputStream->writeEndElement();
        aOutputStream->writeStartElement(CONNECTIONS_LABEL);
        {
            foreach (Connection *connection, mConnections)
            {
                connection->save(aOutputStream);
            }
        }
        aOutputStream->writeEndElement();
        aOutputStream->writeStartElement(TRANSITIONS_LABEL);
        {
            foreach (TerminalTransition *transition, mTTransitions)
            {
                transition->save(aOutputStream);
            }
            foreach (NonTerminalTransition *transition, mNTTransitions)
            {
                transition->save(aOutputStream);
            }
        }
        aOutputStream->writeEndElement();
        ProjectNamedObject::save(aOutputStream);
    }
    aOutputStream->writeEndElement();
}
