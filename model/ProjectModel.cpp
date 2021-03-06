#include "ElementSort.h"
#include "NetClass.h"
#include "ProjectModel.h"
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
const QString AXIOM_CLASS = "ax_class";
const QString AXIOM_NET = "ax_net";


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
    mAxiom.first = 0;
    mAxiom.second = 0;
}

ProjectModel::ProjectModel(QXmlStreamReader *aInputStream)
{
    load(aInputStream);
}

ProjectModel::~ProjectModel()
{

}

///---------------------------------XML---------------------------------------

void ProjectModel::save(QXmlStreamWriter *aOutputStream) const
{
    aOutputStream->writeStartDocument();
    {
        aOutputStream->writeStartElement(PROJECT_LABEL);
        {
            aOutputStream->writeAttribute(PROJECT_NAME, mName);
            aOutputStream->writeAttribute(MAX_ID, QString::number(mMaxID));
            aOutputStream->writeAttribute(AXIOM_CLASS, QString::number(mAxiom.first));
            aOutputStream->writeAttribute(AXIOM_NET, QString::number(mAxiom.second));
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
            else if( name == AXIOM_CLASS)
            {
                mAxiom.first = value.toULongLong();
            }
            else if( name == AXIOM_NET)
            {
                mAxiom.second = value.toULongLong();
            }
        }
        if(aInputStream->readNextStartElement()
                && aInputStream->name() == SORTS)
        {
            while (aInputStream->readNextStartElement()
                   && !(aInputStream->isEndElement()
                        && aInputStream->name() == SORTS)
                   )
            {
                ElementSort *sort = new ElementSort(aInputStream);
                mSorts.insert(sort->ID(), sort);
            }
        }
        if(aInputStream->readNextStartElement()
                && aInputStream->name() == NET_CLASSES)
        {
            while (aInputStream->readNextStartElement()
                   && !(aInputStream->isEndElement()
                        && aInputStream->name() == NET_CLASSES))
            {
                NetClass *netClass = new NetClass(aInputStream);
                mClasses.insert(netClass->ID(), netClass);
            }
        }
    }
    aInputStream->skipCurrentElement();
}

QPair<quint64, quint64> ProjectModel::getAxiom() const
{
    return mAxiom;
}

void ProjectModel::setAxiom(const QPair<quint64, quint64> &axiom)
{
    mAxiom = axiom;
}

///---------------------------------CHILDREN---------------------------------------

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
    return (mClasses.value(aID, 0));
}

ElementSort *ProjectModel::getSortByID(const quint64 &aID)
{
    return mSorts.value(aID, 0);
}

quint64 ProjectModel::createElementSort(QString aSortName)
{
    foreach (ElementSort *sort, mSorts.values())
    {
        if(sort->name() == aSortName)
        {
            return 0;
        }
    }

    quint64 id = generateID();
    mSorts.insert(id, new ElementSort(aSortName, id));
    return id;
}

bool ProjectModel::setNetClassName(const quint64 &aID, const QString &aClassName)
{
    ProjectNamedObject *classWithID = 0;
    foreach (ProjectNamedObject *netClass, mClasses.values())
    {
        if(netClass->name() == aClassName
                && netClass->ID()!= aID)
        {
            return false;
        }
        if(netClass->ID() == aID)
        {
            classWithID = netClass;
        }
    }
    if(classWithID)
    {
        classWithID->setName(aClassName);
        return true;
    }
    else
    {
        return false;
    }
}

bool ProjectModel::setSortName(const quint64 &aID, const QString &aSortName)
{
    ProjectNamedObject *sortWithID = 0;
    foreach (ProjectNamedObject *sort, mSorts.values())
    {
        if(sort->name() == aSortName
                && sort->ID()!= aID)
        {
            return false;
        }
        if(sort->ID() == aID)
        {
            sortWithID = sort;
        }
    }
    if(sortWithID)
    {
        sortWithID->setName(aSortName);
        return true;
    }
    else
    {
        return false;
    }
}

QList<quint64> ProjectModel::getNetClassesIDs()
{
    return mClasses.keys();
}

QList<NetClass *> ProjectModel::getNetClasses()
{
    return mClasses.values();
}

QList<quint64> ProjectModel::getSortsIDs()
{
    return mSorts.keys();
}

///---------------------------------PROPERTIES---------------------------------------

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
