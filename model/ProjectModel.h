#ifndef PROJECTMODEL_H
#define PROJECTMODEL_H

#include <QtCore>
class NetClass;
class ElementSort;

class ElementSort;
class NetClass;
class ProjectModel;
class ProjectNamedObject;

/**
 * @brief The ProjectModelDestroyer class
 * удаляет объект модели при завершении работы приложения.
 */
class ProjectModelDestroyer
{
    private:
        ProjectModel *mInstance;
    public:
        ~ProjectModelDestroyer();
        void initialize(ProjectModel *aProjectModel);
};

/**
 * @brief The ProjectModel class
 * Хранит объекты модели и отвечает за их корректное сохранение/восстановление.
 */
class ProjectModel
{
    public:
        static ProjectModel& newProject(const QString &aName);
        static ProjectModel& newProject(QXmlStreamReader *aInputStream);
        static ProjectModel& getInstance();
        void save(QXmlStreamWriter *aOutputStream) const;

        NetClass *getNetClassByID(const quint64 &aID);
        ElementSort *getSortByID(const quint64 &aID);

        quint64 createNetClass(QString aClassName);
        quint64 createElementSort(QString aSortName);

        bool setNetClassName(const quint64 &aID, const QString &aClassName);
        bool setSortName(const quint64 &aID, const QString &aSortName);

        QList<quint64> getNetClassesIDs();
        QList<NetClass *> getNetClasses();
        QList<quint64> getSortsIDs();

        QString getName() const;
        void setName(const QString &name);
        /**
         * @brief generateID
         * @return Уникальный ID объекта.
         */
        quint64 generateID();

    protected:
        ProjectModel(const QString &aName);
        ProjectModel(QXmlStreamReader *aInputStream);

        ~ProjectModel();

        void load(QXmlStreamReader *aInputStream);

        friend class ProjectModelDestroyer;
    private:
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
