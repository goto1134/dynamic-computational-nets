#include "NonTerminalTransition.h"

const QString NET_ID = "net_id";
const QString CLASS_ID = "class_id";
const QString TIME = "time";
const QString CONTROL_CONNECTIONS = "control_connections";

NonTerminalTransition::NonTerminalTransition(const quint64 &aID, const QPointF &aPoint)
    :TerminalTransition(ProjectObject::NTTransition, aID, aPoint)
{
    mNetID = 0;
    mClassID = 0;
}

NonTerminalTransition::NonTerminalTransition(QXmlStreamReader *aInputStream)
{
    load(aInputStream);
}

quint64 NonTerminalTransition::netID() const
{
    return mNetID;
}

void NonTerminalTransition::setNetID(const quint64 &netID)
{
    mNetID = netID;
    emit netChanged(mClassID, mNetID);
}

void NonTerminalTransition::setNetID(const quint64 &classID, const quint64 &netID)
{
    mClassID = classID;
    setNetID(netID);
}

void NonTerminalTransition::setConnectionControl(const quint64 &aID, const bool &aIsControl)
{
    if(aIsControl)
    {
        removeInputConnectionID(aID);
        mControlConnections.insert(aID);
    }
    else
    {
        mControlConnections.remove(aID);
        addInputConnectionID(aID);
    }
}

void NonTerminalTransition::load(QXmlStreamReader *aInputStream)
{
    if(aInputStream->isStartElement()
            && aInputStream->name() == NON_TERMINAL_TRANSITION)
    {
        foreach(QXmlStreamAttribute attribute,  aInputStream->attributes())
        {
            if(attribute.name().toString() == NET_ID)
            {
                mNetID = attribute.value().toULongLong();
            }
            if(attribute.name().toString() == CLASS_ID)
            {
                mClassID = attribute.value().toULongLong();
            }
            if(attribute.name().toString() == TIME)
            {
                mTime = attribute.value().toInt();
            }
        }
        getConnectionsIDs(&mControlConnections, aInputStream, CONTROL_CONNECTIONS);
        aInputStream->readNextStartElement();
        TerminalTransition::load(aInputStream);
    }
    aInputStream->skipCurrentElement();
}

void NonTerminalTransition::save(QXmlStreamWriter *aOutputStream) const
{
    aOutputStream->writeStartElement(NON_TERMINAL_TRANSITION);
    {
        aOutputStream->writeAttribute(NET_ID, QString::number(mNetID));
        aOutputStream->writeAttribute(CLASS_ID, QString::number(mClassID));
        aOutputStream->writeAttribute(TIME, QString::number(mTime));
        saveConnectionIDs(mControlConnections, aOutputStream, CONTROL_CONNECTIONS);
        TerminalTransition::save(aOutputStream);
    }
    aOutputStream->writeEndElement();
}

quint64 NonTerminalTransition::classID() const
{
    return mClassID;
}

int NonTerminalTransition::time() const
{
    return mTime;
}

void NonTerminalTransition::setTime(int time)
{
    mTime = time;
}
