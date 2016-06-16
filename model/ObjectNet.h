#ifndef OBJECTNET_H
#define OBJECTNET_H

#include <QObject>

#include <model/base/ProjectNamedObject.h>
class Place;
class TerminalTransition;
class NonTerminalTransition;
class Connection;

class ObjectNet: public ProjectNamedObject
{
       Q_OBJECT
    public:
        explicit ObjectNet(const QString &aName, const quint64 &aID, const quint64 &aNetClassID);
        explicit ObjectNet(QXmlStreamReader *aInputStream);

        void load(QXmlStreamReader *aInputStream);
        void save(QXmlStreamWriter *aOutputStream) const;
        void netClassPlaceRemoved(const quint64 &aID);
        quint64 netClassID() const;

        QList<Place *> places();
        QList<TerminalTransition *> transitions();
        QList<NonTerminalTransition *> nonTerminalTransitions();
        TerminalTransition *getTransitionByID(const quint64 &aID);
        QList<Connection *> connections();
        Place *getPlaceByID(const quint64 &aID);
        Place *addPlace(const QPointF &aPoint);
        TerminalTransition *addTransition(const QPointF &aPoint);
        NonTerminalTransition *addNonTerminalTransition(const QPointF &aPoint);
        Connection *addConnection(const quint64 &aStartID, const quint64 &aEndID);
        Connection *getConnectionByID(const quint64 &aID);
        void deletePlace(const quint64 &aID);
    private:
        quint64 mNetClassID;
        QMap<quint64, Place *> mPlaces;
        QMap<quint64, Connection *> mConnections;
        QMap<quint64, TerminalTransition *> mTTransitions;
        QMap<quint64, NonTerminalTransition *> mNTTransitions;
};

#endif // OBJECTNET_H
