#include "ProjectTreeModel.h"
#include <QDebug>
#include <QColor>

#include <model/ProjectModel.h>

ProjectTreeModel::ProjectTreeModel(QObject *aParent)
    : QAbstractItemModel(aParent)
{
    ProjectModel &model = ProjectModel::getInstance();
    mRootItem = new TreeItem(model.getName());

    mSortParent = new TreeItem("Element Sorts", mRootItem, TreeItem::SortContainer);
    mRootItem->appendChild(mSortParent);
    foreach(quint64 sortID, model.getSortsIDs())
    {
        mSortParent->appendChild(new TreeItem(sortID, mSortParent, TreeItem::Sort));
    }


    mObjectNetsParent = new TreeItem("Object-Nets", mRootItem, TreeItem::ObjectNetsContainer);
    mRootItem->appendChild(mObjectNetsParent);

    mAxiom = new TreeItem("Axiom", mObjectNetsParent, TreeItem::Axiom);
    mObjectNetsParent->appendChild(mAxiom);
    foreach(quint64 classID, model.getNetClassesIDs())
    {
        TreeItem *netClass = new TreeItem(classID, mObjectNetsParent, TreeItem::Class);
        mObjectNetsParent->appendChild(netClass);
        foreach (quint64 objectNetID, model.getNetClassByID(classID)->getObjectNetsIDs())
        {
            netClass->appendChild(new TreeItem(objectNetID, netClass, TreeItem::ObjectNet));
        }
    }
}

ProjectTreeModel::~ProjectTreeModel()
{
    delete mRootItem;
}

QVariant ProjectTreeModel::data(const QModelIndex &aIndex, int aRole) const
{
    if (!aIndex.isValid())
    {
        return QVariant();
    }
    TreeItem *item = static_cast<TreeItem*>(aIndex.internalPointer());
    if (aRole == Qt::DisplayRole)
    {
        return item->getData(aIndex.column());
    }
    else if(aRole == Qt::DecorationRole)
    {
        return item->getImage();;
    }
    else if(aRole == Qt::UserRole)
    {
        return item->getItemType();
    }
    else if(aRole == Qt::UserRole + 1)
    {
        return QVariant(item->getObjectID());
    }
    else if(aRole == Qt::ForegroundRole)
    {
        QVariant color = item->getTextColor();
        return color;
    }
    else
    {
        return QVariant();
    }
}

Qt::ItemFlags ProjectTreeModel::flags(const QModelIndex &aIndex) const
{
    if (!aIndex.isValid())
    {
        return 0;
    }

    return QAbstractItemModel::flags(aIndex);
}



QVariant ProjectTreeModel::headerData(int aSection, Qt::Orientation aOrientation, int aRole) const
{
    if (aOrientation == Qt::Horizontal && aRole == Qt::DisplayRole)
    {
        return mRootItem->getData(aSection);
    }

    return QVariant();
}

QModelIndex ProjectTreeModel::index(int aRow, int aColumn, const QModelIndex &aParent) const
{
    if (!hasIndex(aRow, aColumn, aParent))
    {
        return QModelIndex();
    }

    TreeItem *parentItem;

    if (!aParent.isValid())
    {
        parentItem = mRootItem;
    }
    else
    {
        parentItem = static_cast<TreeItem*>(aParent.internalPointer());
    }

    TreeItem *childItem = parentItem->child(aRow);
    if (childItem)
    {
        return createIndex(aRow, aColumn, childItem);
    }
    else
    {
        return QModelIndex();
    }
}

QModelIndex ProjectTreeModel::parent(const QModelIndex &aIndex) const
{
    if (!aIndex.isValid())
    {
        return QModelIndex();
    }

    TreeItem *childItem = static_cast<TreeItem*>(aIndex.internalPointer());
    TreeItem *parentItem = childItem->getParentItem();

    if (parentItem == mRootItem)
    {
        return QModelIndex();
    }

    return createIndex(parentItem->row(), 0, parentItem);
}

/**
 * @brief ProjectTreeModel::rowCount
 * @param aParent
 * @return
 */
int ProjectTreeModel::rowCount(const QModelIndex &aParent) const
{
    TreeItem *parentItem;
    if (aParent.column() > 0)
    {
        return 0;
    }

    if (!aParent.isValid())
    {
        parentItem = mRootItem;
    }
    else
    {
        parentItem = static_cast<TreeItem*>(aParent.internalPointer());
    }

    return parentItem->childCount();
}

/**
 * @brief ProjectTreeModel::columnCount
 * @param aParent
 * @return
 */
int ProjectTreeModel::columnCount(const QModelIndex &aParent) const
{
    if (aParent.isValid())
    {
        return static_cast<TreeItem*>(aParent.internalPointer())->dataLength();
    }
    else
    {
        return mRootItem->dataLength();
    }
}

void ProjectTreeModel::addElementSort(QString aSortName)
{
    qDebug() << "add Sort " << aSortName;
    if(quint64 id = ProjectModel::getInstance().createElementSort(aSortName))
    {
        emit layoutAboutToBeChanged();
        //        mElementSorts.insert(aSortName, new ElementSort(aSortName));
        mSortParent->appendChild(new TreeItem(id, mSortParent, TreeItem::Sort));
        emit layoutChanged();
    }
}

void ProjectTreeModel::addNetClass(QString aClassName)
{
    qDebug() << "add Net Class " << aClassName;
    if(quint64 id = ProjectModel::getInstance().createNetClass(aClassName))
    {
        emit layoutAboutToBeChanged();

        mObjectNetsParent->appendChild(new TreeItem(id, mObjectNetsParent, TreeItem::Class));

        emit layoutChanged();
    }
}

void ProjectTreeModel::addObjectNet(QModelIndex &aParent, QString aNetName)
{
    if(aParent.isValid())
    {
        TreeItem *parent = static_cast<TreeItem*>(aParent.internalPointer());
        if(parent->getItemType() == TreeItem::Class)
        {
            if(quint64 id =
                    ProjectModel::getInstance()
                    .getNetClassByID(parent->getObjectID())
                    ->createObjectNet(aNetName))
            {
                emit layoutAboutToBeChanged();

                parent->appendChild(new TreeItem(id, parent, TreeItem::ObjectNet));

                emit layoutChanged();
            }
        }
    }
}
