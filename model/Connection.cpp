#include "Connection.h"

#include "ObjectNet.h"
#include "Place.h"
#include "NonTerminalTransition.h"

const QString START_ID = "start";
const QString END_ID = "end";
const QString TIME_LABEL = "time";
const QString CONNECTION_TYPE = "connection_type";

Connection::Connection(const quint64 &aID,
                       const quint64 &aStartID,
                       const quint64 &aEndID,
                       const Connection::ConnectionVariant &aType,
                       ObjectNet *aNet)
    :ProjectObject(ProjectObject::ConnectionType, aID)
{
    mResources = 1;
    if(aType == InputTerminal
            || aType == OutputTerminal)
    {
        mTime = 1;
    }
    else
    {
        mTime = 0;
    }
    mStartID = aStartID;
    mEndID = aEndID;
    mConnectionType = aType;
    if(aType == Control
            || aType == InputNonTerminal)
    {
        mNet = aNet;
    }
}

Connection::Connection(QXmlStreamReader *aInputStream, ObjectNet *aNet)
{
    load(aInputStream);
    if(mConnectionType == InputNonTerminal)
    {
        mNet = aNet;
    }
}

void Connection::load(QXmlStreamReader *aInputStream)
{
    if(aInputStream->isStartElement()
            && aInputStream->name() == CONNECTION_LABEL)
    {
        foreach(QXmlStreamAttribute attribute,  aInputStream->attributes())
        {
            QString name = attribute.name().toString();
            if(name == START_ID)
            {
                mStartID = attribute.value().toULongLong();
            }
            else if( name == END_ID)
            {
                mEndID = attribute.value().toULongLong();
            }
            else if( name == RESOURCE_NUMBER_LABEL)
            {
                mResources = attribute.value().toUInt();
            }
            else if( name == TIME_LABEL)
            {
                mTime = attribute.value().toUInt();
            }
            else if(name == CONNECTION_TYPE)
            {
                mConnectionType = static_cast<ConnectionVariant>(attribute.value().toUInt());
            }
        }
        aInputStream->readNextStartElement();
        ProjectObject::load(aInputStream);
    }
    aInputStream->skipCurrentElement();
}

void Connection::save(QXmlStreamWriter *aOutputStream) const
{
    aOutputStream->writeStartElement(CONNECTION_LABEL);
    {
        aOutputStream->writeAttribute(CONNECTION_TYPE, QString::number(mConnectionType));
        aOutputStream->writeAttribute(START_ID, QString::number(mStartID));
        aOutputStream->writeAttribute(END_ID, QString::number(mEndID));
        aOutputStream->writeAttribute(TIME_LABEL, QString::number(mTime));
        aOutputStream->writeAttribute(RESOURCE_NUMBER_LABEL, QString::number(mResources));
        ProjectObject::save(aOutputStream);
    }
    aOutputStream->writeEndElement();
}

quint64 Connection::startID() const
{
    return mStartID;
}

void Connection::setStartID(const quint64 &startID)
{
    mStartID = startID;
}

quint64 Connection::endID() const
{
    return mEndID;
}

void Connection::setEndID(const quint64 &endID)
{
    mEndID = endID;
}

int Connection::resources() const
{
    return mResources;
}

void Connection::setResources(int resources)
{
    mResources = resources;
    emit dataChanged();
}

int Connection::time() const
{
    if(mConnectionType == InputTerminal
            || mConnectionType == OutputTerminal)
    {
        return mTime;
    }
    else
    {
        return 0;
    }
}

void Connection::setTime(int time)
{
    mTime = time;
    emit dataChanged();
}

Connection::ConnectionVariant Connection::connectionType() const
{
    return mConnectionType;
}

void Connection::switchControl()
{
    NonTerminalTransition *transition = static_cast<NonTerminalTransition *>(mNet->getTransitionByID(mEndID));
    if(mConnectionType == InputNonTerminal && mNet->getPlaceByID(mStartID)->sortID() == 0)
    {
        transition->setConnectionControl(ID(), true);
        mConnectionType = Control;
    }
    else if(mConnectionType == Control)
    {
        transition->setConnectionControl(ID(), false);
        mConnectionType = InputNonTerminal;
    }
}
