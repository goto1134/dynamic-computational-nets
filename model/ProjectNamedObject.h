#ifndef PROJECTNAMEDOBJECT_H
#define PROJECTNAMEDOBJECT_H

#include "ProjectObject.h"

const QString PLACES_LABEL = "places";

class ProjectNamedObject : public ProjectObject
{
    public:
        explicit ProjectNamedObject();
        explicit ProjectNamedObject(const ProjectObject::Type &aType, const QString& aName);
        explicit ProjectNamedObject(QXmlStreamReader *aInputStream);

        void load(QXmlStreamReader *aInputStream);
        void save(QXmlStreamWriter *aOutputStream) const;
        QString name() const;
        void setName(const QString &aName);

    private:
        QString mName;
    signals:

    public slots:
};

#endif // PROJECTNAMEDOBJECT_H