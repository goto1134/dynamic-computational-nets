#include "ProjectModel.h"
#include "ElementSort.h"
#include "NetClass.h"
#include <QDebug>

///
/// PROJECT MODEL DESTROYER
///

ProjectModelDestroyer::~ProjectModelDestroyer()
{
    delete mInstance;
}

void ProjectModelDestroyer::initialize(ProjectModel *aProjectModel)
{
    mInstance = aProjectModel;;
}

///
/// PROJECT MODEL
///

ProjectModel* ProjectModel::mProjectModel = 0;
ProjectModelDestroyer ProjectModel::mDestroyer;

const QString PROJECT_LABEL = "project";
const QString NET_CLASSES = "net_classes";
const QString SORTS = "sorts";
const QString MAX_ID = "max_id";
const QString PROJECT_NAME = "project_name";

ProjectModel &ProjectModel::newProject(const QString &aName)
{

    mProjectModel = new ProjectModel(aName);
    mDestroyer.initialize( mProjectModel);
    return *mProjectModel;
}

ProjectModel &ProjectModel::newProject(QXmlStreamReader *aInputStream)
{
    mProjectModel = new ProjectModel(aInputStream);
    mDestroyer.initialize( mProjectModel);
    return *mProjectModel;
}

ProjectModel &ProjectModel::getInstance()
{
    return *mProjectModel;
}

ProjectModel::ProjectModel(const QString &aName)
{
    mName = aName;
    mMaxID = 0;
    ElementSort *naturalSort = new ElementSort("nat", 0);
    mSorts.insert(0, naturalSort);
}

ProjectModel::ProjectModel(QXmlStreamReader *aInputStream)
{
    load(aInputStream);
}

ProjectModel::~ProjectModel()
{

}

void ProjectModel::save(QXmlStreamWriter *aOutputStream) const
{
    aOutputStream->writeStartDocument();
    {
        aOutputStream->writeStartElement(PROJECT_LABEL);
        {
            aOutputStream->writeAttribute(PROJECT_NAME, mName);
            aOutputStream->writeAttribute(MAX_ID, QString::number(mMaxID));
            aOutputStream->writeStartElement(SORTS);
            {
                foreach (ElementSort *sort, mSorts.values())
                {
                    sort->save(aOutputStream);
                }
            }
            aOutputStream->writeEndElement();
            aOutputStream->writeStartElement(NET_CLASSES);
            {
                foreach (NetClass *netClass, mClasses.values())
                {
                    netClass->save(aOutputStream);
                }
            }
            aOutputStream->writeEndElement();
        }
        aOutputStream->writeEndElement();
    }
    aOutputStream->writeEndDocument();
}

void ProjectModel::load(QXmlStreamReader *aInputStream)
{
    if(aInputStream->readNextStartElement()
            && aInputStream->name() == PROJECT_LABEL)
    {
        qDebug() << aInputStream->name();
        foreach(QXmlStreamAttribute attribute,  aInputStream->attributes())
        {
            QString name = attribute.name().toString();
            QStringRef value = attribute.value();
            if(name == PROJECT_NAME)
            {
                mName = value.toString();
            }
            else if( name == MAX_ID)
            {
                mMaxID = value.toULongLong();
            }
        }
        if(aInputStream->readNextStartElement()
                && aInputStream->name() == SORTS)
        {
            qDebug() << aInputStream->name();
            while (aInputStream->readNextStartElement()
                   && !(aInputStream->isEndElement()
                        && aInputStream->name() == SORTS)
                   )
            {
                qDebug() << aInputStream->name();
                ElementSort *sort = new ElementSort(aInputStream);
                mSorts.insert(sort->ID(), sort);
            }
        }
        qDebug() << aInputStream->name();
        if(aInputStream->readNextStartElement()
                && aInputStream->name() == NET_CLASSES)
        {
            qDebug() << aInputStream->name();
            while (aInputStream->readNextStartElement()
                   && !(aInputStream->isEndElement()
                        && aInputStream->name() == NET_CLASSES))
            {
                qDebug() << aInputStream->name();
                NetClass *netClass = new NetClass(aInputStream);
                mClasses.insert(netClass->ID(), netClass);
            }
        }
        qDebug() << aInputStream->name();
    }
    aInputStream->skipCurrentElement();
}


quint64 ProjectModel::createNetClass(QString aClassName)
{
    foreach (NetClass *netClass, mClasses.values())
    {
        if(netClass->name() == aClassName)
        {
            return 0;
        }
    }

    quint64 id = generateID();
    mClasses.insert(id, new NetClass(aClassName, id));
    return id;
}

NetClass *ProjectModel::getNetClassByID(const quint64 &aID)
{
    if(mClasses.keys().contains(aID))
    {
        return (mClasses.value(aID));
    }
    else
    {
        return 0;
    }
}

ElementSort *ProjectModel::getSortByID(const quint64 &aID)
{
    return mSorts.value(aID, 0);
}

quint64 ProjectModel::createElementSort(QString aSortName)
{

}

QList<quint64> ProjectModel::getNetClassesIDs()
{
    return mClasses.keys();
}

QList<quint64> ProjectModel::getSortsIDs()
{
    return mSorts.keys();
}

quint64 ProjectModel::generateID()
{
    return ++mMaxID;
}

QString ProjectModel::getName() const
{
    return mName;
}

void ProjectModel::setName(const QString &name)
{
    mName = name;
}


