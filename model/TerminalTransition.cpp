#include "TerminalTransition.h"


TerminalTransition::TerminalTransition()
{

}

TerminalTransition::TerminalTransition(const quint64 &aID, const QPointF &aPoint)
    :ProjectGraphicsObject(ProjectObject::TTransition, aID, aPoint)
{

}

TerminalTransition::TerminalTransition(QXmlStreamReader *aInputStream)
{
    load(aInputStream);
}

void TerminalTransition::load(QXmlStreamReader *aInputStream)
{
    if(aInputStream->isStartElement()
            && aInputStream->name() == TERMINAL_TRANSITION)
    {
        if(aInputStream->readNextStartElement()
                && aInputStream->name() == CONNECTIONS_LABEL)
        {
            getConnectionsIDs(&mInputConnectionsIDs, aInputStream, INPUT_LABEL);
            getConnectionsIDs(&mOutputConnectionsIDs, aInputStream, OUTPUT_LABEL);
            aInputStream->skipCurrentElement();
        }
    }
    aInputStream->readNextStartElement();
    ProjectGraphicsObject::load(aInputStream);
    aInputStream->skipCurrentElement();
}

void TerminalTransition::save(QXmlStreamWriter *aOutputStream) const
{
    aOutputStream->writeStartElement(TERMINAL_TRANSITION);
    {
        aOutputStream->writeStartElement(CONNECTIONS_LABEL);
        {
            saveConnectionIDs(mInputConnectionsIDs, aOutputStream, INPUT_LABEL);
            saveConnectionIDs(mOutputConnectionsIDs, aOutputStream, OUTPUT_LABEL);
        }
        aOutputStream->writeEndElement();
        ProjectGraphicsObject::save(aOutputStream);
    }
    aOutputStream->writeEndElement();
}

void TerminalTransition::addInputConnectionID(const quint64 &connectionID)
{
    mInputConnectionsIDs.insert(connectionID);
}

void TerminalTransition::addOutputConnectionID(const quint64 &connectionID)
{
    mOutputConnectionsIDs.insert(connectionID);
}

TerminalTransition::TerminalTransition(const ProjectObject::Type &aType, const quint64 &aID, const QPointF &aPoint)
    :ProjectGraphicsObject(aType, aID, aPoint)
{

}

void TerminalTransition::getConnectionsIDs(QSet<quint64> *aConnectionSet, QXmlStreamReader *aInputStream, QString name)
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

void TerminalTransition::saveConnectionIDs(QSet<quint64> aSource, QXmlStreamWriter *aOutputStream, QString aName) const
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

