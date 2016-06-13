#include "Connection.h"

const QString START_ID = "start";
const QString END_ID = "end";
const QString TIME_LABEL = "time";
const QString CONTROL_LABEL = "isControl";


Connection::Connection(const quint64 &aID)
    :ProjectObject(ProjectObject::ConnectionType, aID)
{
    mResources = 0;
    mTime = 0;
    mIsControl = false;
}

Connection::Connection(QXmlStreamReader *aInputStream)
{
    load(aInputStream);
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
           else if( name == CONTROL_LABEL)
           {
               mIsControl = attribute.value().toUInt();
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
        aOutputStream->writeAttribute(START_ID, QString::number(mStartID));
        aOutputStream->writeAttribute(END_ID, QString::number(mEndID));
        aOutputStream->writeAttribute(TIME_LABEL, QString::number(mTime));
        aOutputStream->writeAttribute(RESOURCE_NUMBER_LABEL, QString::number(mResources));
        aOutputStream->writeAttribute(CONTROL_LABEL, QString::number(mIsControl));
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
}

int Connection::time() const
{
    return mTime;
}

void Connection::setTime(int time)
{
    mTime = time;
}

bool Connection::isControl() const
{
    return mIsControl;
}

void Connection::setIsControl(bool isControl)
{
    mIsControl = isControl;
}
