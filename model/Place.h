#ifndef PLACE_H
#define PLACE_H

#include <QSet>
#include "ProjectObject.h"

class Connection;
const QString PLACE_LABEL = "place";

class Place : public ProjectObject
{
    public:
        explicit Place(const quint64 &aID);
        explicit Place(QXmlStreamReader *aInputStream);

        void load(QXmlStreamReader *aInputStream);
        void save(QXmlStreamWriter *aOutputStream) const;

        quint64 sortID() const;
        void setSortID(const quint64 &sortID);

        quint64 resourceNumber() const;
        void setResourceNumber(const quint64 &resourceNumber);

        void addInputConnectionID(const quint64 &connectionID);
        void addOutputConnectionID(const quint64 &connectionID);

    private:
        quint64 mSortID;
        QSet<quint64> mInputConnIDs;
        QSet<quint64> mOutputConnIDs;
        quint64 mResourceNumber;

        void saveConnectionIDs(QSet<quint64> source, QXmlStreamWriter *aOutputStream, QString name) const;

        void getConnectionsIDs(QSet<quint64> *aConnectionSet, QXmlStreamReader *aInputStream, QString name);

    signals:

    public slots:

};
#endif // PLACE_H
