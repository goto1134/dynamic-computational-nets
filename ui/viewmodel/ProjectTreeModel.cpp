#include "ProjectTreeModel.h"
#include <QDebug>
#include <QColor>

ProjectTreeModel::ProjectTreeModel(QString aProjectName, QObject *aParent) : QAbstractItemModel(aParent)
{
    mRootItem = new TreeItem(aProjectName);

    mSortParent = new TreeItem("Element Sorts", mRootItem, TreeItem::SortContainer);
    mRootItem->appendChild(mSortParent);
    mElementSorts.insert("nat",new ElementSort("nat"));
    mSortParent->appendChild(new TreeItem("nat", mSortParent, TreeItem::Sort));

    mObjectNetsParent = new TreeItem("Object-Nets", mRootItem, TreeItem::ObjectNetsContainer);
    mRootItem->appendChild(mObjectNetsParent);

    mAxiom = new TreeItem("Axiom", mObjectNetsParent, TreeItem::Axiom);
    mObjectNetsParent->appendChild(mAxiom);
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

    if (aRole == Qt::DisplayRole)
    {
        TreeItem *item = static_cast<TreeItem*>(aIndex.internalPointer());
        return item->getData(aIndex.column());
    }
    else if(aRole == Qt::UserRole)
    {
        TreeItem *item = static_cast<TreeItem*>(aIndex.internalPointer());
        return item->getItemType();
    }
    else if(aRole == Qt::ForegroundRole)
    {
        TreeItem *item = static_cast<TreeItem*>(aIndex.internalPointer());
        if(item->getItemType().toInt() == TreeItem::Sort)
        {
            return QColor(Qt::black);
        }
        else
        {
            return QVariant();
        }
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
    if(!mElementSorts.contains(aSortName))
    {
        emit layoutAboutToBeChanged();
        mElementSorts.insert(aSortName, new ElementSort(aSortName));
        mSortParent->appendChild(new TreeItem(aSortName, mSortParent, TreeItem::Sort));
        emit layoutChanged();
    }
}

void ProjectTreeModel::addNetClass(QString aClassName)
{
    qDebug() << "add Sort " << aClassName;
    if(!mNetClasses.contains(aClassName))
    {
        emit layoutAboutToBeChanged();
        mNetClasses.insert(aClassName, new NetClass(aClassName));
        mObjectNetsParent->appendChild(new TreeItem(aClassName, mObjectNetsParent, TreeItem::Class));
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
                NetClass *netClass = mNetClasses.value(parent->getData(0).toString());
                if(netClass->createObjectNet(aNetName))
                {
                    emit layoutAboutToBeChanged();
                    parent->appendChild(new TreeItem(aNetName, parent, TreeItem::ObjectNet));
                    emit layoutChanged();
                }
            }
        }
}
