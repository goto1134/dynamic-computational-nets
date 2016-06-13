#include "NonTerminalTransition.h"

const QString NET_ID = "net_id";

NonTerminalTransition::NonTerminalTransition(const quint64 &aID)
    :TerminalTransition(ProjectObject::NTTransition, aID)
{

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
       }
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
    }
    TerminalTransition::save(aOutputStream);
    aOutputStream->writeEndElement();
}
