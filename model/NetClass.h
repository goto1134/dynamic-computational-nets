#ifndef NETCLASS_H
#define NETCLASS_H

#include <QMap>
#include "ProjectNamedObject.h"

class Place;
class ObjectNet;
//class Place;

class NetClass : public ProjectNamedObject
{
    public:
        explicit NetClass(const QString& aName, const quint64 &aID);
        explicit NetClass(QXmlStreamReader *aInputStream);

        quint64 createObjectNet(QString aNetName);
        ObjectNet* getObjectNetByID(const quint64 &aID);
        QList<quint64> getObjectNetsIDs() const;
        void load(QXmlStreamReader *aInputStream);
        void save(QXmlStreamWriter *aOutputStream) const;

        bool setNetName(const quint64 &aNetID, const QString &aNewName);
        QList<Place *> places();

        int getInputPlaceNumber();
        int getOutputPlaceNumber();
        void setInputPlaceNumber(const int &aInputNumber);
        void setOutputPlaceNumber(const int &aOutputNumber);

    private:
        void setPlaceNumber(const int &aInputNumber, const bool &aInput, QMap<quint64, Place *> *aPlacesMap);
        void removePlace(quint64 &aID, QMap<quint64, Place *> *aPlacesMap);

        QMap<quint64, ObjectNet *> mObjectNets;
        QMap<quint64, Place *> mInputPlaces;
        QMap<quint64, Place *> mOutputPlaces;

        void writePlaces(QXmlStreamWriter *aOutputStream, QMap<quint64, Place *> aPlacesMap, QString aTagName) const;
        void getPlaces(QString aTagName, QXmlStreamReader *aInputStream, QMap<quint64, Place *> *aPlacesMap);
};

#endif // NETCLASS_H
