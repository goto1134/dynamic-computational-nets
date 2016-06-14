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
        void netClassPlaceRemoved(const quint64 &aID);
        quint64 netClassID() const;

        QList<Place *> places();
        Place *addPlace(const QPointF &aPoint);
        void deletePlace(const quint64 &aID);

    private:
        quint64 mNetClassID;
        QMap<quint64, Place *> mPlaces;
        QMap<quint64, Connection *> mConnections;
        QSet<TerminalTransition *> mTTransitions;
        QSet<NonTerminalTransition *> mNTTransitions;
};

#endif // OBJECTNET_H
