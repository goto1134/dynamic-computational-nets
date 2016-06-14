#ifndef PROJECTOBJECT_H
#define PROJECTOBJECT_H

#include <QtCore>

const QString ID_LABEL = "id";
const QString INPUT_LABEL = "input";
const QString OUTPUT_LABEL = "output";
const QString RESOURCE_NUMBER_LABEL = "resource_number";
const QString CONNECTIONS_LABEL = "connections";

class ProjectObject : public QObject
{
        Q_OBJECT
    public:
        enum Type
        {
            Sort,
            NetClass,
            Net,
            PlaceType,
            ConnectionType,
            TTransition,
            NTTransition,
        };
        explicit ProjectObject();
        explicit ProjectObject(const Type &aType);
        explicit ProjectObject(const Type &aType, const quint64 &aID);
        explicit ProjectObject(QXmlStreamReader *aInputStream);
        void save(QXmlStreamWriter *aOutputStream) const;
        void load(QXmlStreamReader *aInputStream);
        QList<QWidget *> *getPropertyWidgets();
        Type type() const;
        quint64 ID() const;


    protected:
        void setID(const quint64 &aID);
        void setType(const Type &aType);

    private:
        quint64 mID;
        Type mType;
    signals:

    public slots:
};

#endif // PROJECTOBJECT_H
