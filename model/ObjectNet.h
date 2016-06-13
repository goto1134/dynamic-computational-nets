#ifndef OBJECTNET_H
#define OBJECTNET_H

#include <QObject>
#include "ProjectNamedObject.h"
class Place;
class TerminalTransition;
class NonTerminalTransition;
class Connection;

class ObjectNet: public ProjectNamedObject
{
    public:
        explicit ObjectNet(const QString &aName, const quint64 &aID, const quint64 &aNetClassID);
        explicit ObjectNet(QXmlStreamReader *aInputStream);

        void load(QXmlStreamReader *aInputStream);
        void save(QXmlStreamWriter *aOutputStream) const;
        quint64 netClassID() const;

    private:
        quint64 mNetClassID;
        QSet<Place *> mPlaces;
        QSet<Connection *> mConnections;
        QSet<TerminalTransition *> mTTransitions;
        QSet<NonTerminalTransition *> mNTTransitions;
};

#endif // OBJECTNET_H
