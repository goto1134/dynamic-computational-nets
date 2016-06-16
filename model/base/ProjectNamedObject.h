#ifndef PROJECTNAMEDOBJECT_H
#define PROJECTNAMEDOBJECT_H

#include "ProjectObject.h"

const QString PLACES_LABEL = "places";

class ProjectNamedObject : public ProjectObject
{
        Q_OBJECT
    public:
        explicit ProjectNamedObject(const Type &aType, const QString &aName, const quint64 &aID);
        explicit ProjectNamedObject(QXmlStreamReader *aInputStream);

        void load(QXmlStreamReader *aInputStream);
        void save(QXmlStreamWriter *aOutputStream) const;
        QString name() const;
        void setName(const QString &aName);
    protected:
        explicit ProjectNamedObject();
    private:
        QString mName;
    signals:
        nameChanged(const QString &aNewName);
    public slots:
};

#endif // PROJECTNAMEDOBJECT_H
