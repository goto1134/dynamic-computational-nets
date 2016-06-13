#ifndef NETCLASS_H
#define NETCLASS_H

#include <QMap>
#include "ProjectNamedObject.h"

#include "Place.h"
class ObjectNet;
//class Place;

class NetClass : public ProjectNamedObject
{
    public:
        explicit NetClass(const QString& aName, const quint64 &aID);
        explicit NetClass(QXmlStreamReader *aInputStream);

        bool createObjectNet(QString aNetName);
        void load(QXmlStreamReader *aInputStream);
        void save(QXmlStreamWriter *aOutputStream) const;

        void addInputPlace(Place *aPlace);
        void addOutputPlace(Place *aPlace);

    private:
        QMap<QString, ObjectNet*> mObjectNets;
        QSet<Place *> mInputPlaces;
        QSet<Place *> mOutputPlaces;

        void writePlaces(QXmlStreamWriter *aOutputStream, QSet<Place *> aPlacesSet, QString aTagName) const;
        void getPlaces(QString aTagName, QXmlStreamReader *aInputStream, QSet<Place *> *aPlacesSet);
};

#endif // NETCLASS_H
