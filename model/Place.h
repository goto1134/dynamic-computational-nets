#ifndef PLACE_H
#define PLACE_H

#include <QSet>
#include "ProjectGraphicsObject.h"

class Connection;
const QString PLACE_LABEL = "place";

class Place : public ProjectGraphicsObject
{
        Q_OBJECT
    public:
        enum PlaceVariant
        {
            Input,
            Output,
            Common
        };

        explicit Place(const quint64 &aID, const QPointF &aPoint = QPointF(), const PlaceVariant &type = Common);
        explicit Place(QXmlStreamReader *aInputStream);

        void load(QXmlStreamReader *aInputStream);
        void save(QXmlStreamWriter *aOutputStream) const;

        quint64 sortID() const;
        void setSortID(const quint64 &sortID);

        quint64 resourceNumber() const;
        void setResourceNumber(const quint64 &resourceNumber);

        void addInputConnectionID(const quint64 &connectionID);
        void addOutputConnectionID(const quint64 &connectionID);

        void removeInputConnectionID(const quint64 &connectionID);
        void removeOutputConnectionID(const quint64 &connectionID);

        PlaceVariant getPlaceType() const;
        void setPlaceType(const PlaceVariant &placeType);

    private:
        quint64 mSortID;
        QSet<quint64> mInputConnIDs;
        QSet<quint64> mOutputConnIDs;
        quint64 mResourceNumber;
        PlaceVariant mPlaceType;

        void saveConnectionIDs(QSet<quint64> source, QXmlStreamWriter *aOutputStream, QString name) const;

        void getConnectionsIDs(QSet<quint64> *aConnectionSet, QXmlStreamReader *aInputStream, QString name);

    signals:
        void sortChanged(const quint64 &aSort);
        void resourceNumberChanged(const quint64 &aResourceNumber);
    public slots:

};
#endif // PLACE_H
