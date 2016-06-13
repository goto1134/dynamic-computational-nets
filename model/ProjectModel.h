#ifndef PROJECTMODEL_H
#define PROJECTMODEL_H

#include <QtCore>
#include "NetClass.h"
#include "ElementSort.h"

class ElementSort;
class NetClass;
class ProjectModel;

class ProjectModelDestroyer
{
    private:
        ProjectModel *mInstance;
    public:
        ~ProjectModelDestroyer();
        void initialize(ProjectModel *aProjectModel);
};

class ProjectModel
{
    public:
        static ProjectModel& newProject(const QString &aName);
        static ProjectModel& newProject(QXmlStreamReader *aInputStream);
        static ProjectModel& getInstance();
        void save(QXmlStreamWriter *aOutputStream) const;
        quint64 createNetClass(QString aClassName);
        NetClass *getNetClassByID(const quint64 &aID);
        ElementSort *getSortByID(const quint64 &aID);
        quint64 createElementSort(QString aSortName);

        QList<quint64> getNetClassesIDs();
        QList<quint64> getSortsIDs();

        QString getName() const;
        void setName(const QString &name);

    protected:
        ProjectModel(const QString &aName);
        ProjectModel(QXmlStreamReader *aInputStream);

        ~ProjectModel();

        void load(QXmlStreamReader *aInputStream);

        friend class ProjectModelDestroyer;
    private:
        quint64 generateID();
        quint64 mMaxID;
        static ProjectModel *mProjectModel;
        static ProjectModelDestroyer mDestroyer;
        QString mName;
        QMap<quint64, ElementSort *> mSorts;
        QMap<quint64, NetClass *> mClasses;
    signals:

    public slots:
};

#endif // PROJECTMODEL_H
