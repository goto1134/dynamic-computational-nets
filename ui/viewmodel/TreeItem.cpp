#include "TreeItem.h"
#include <QtAlgorithms>
#include <QIcon>

#include <model/ProjectModel.h>
#include <model/ElementSort.h>
#include <model/NetClass.h>
#include <model/ObjectNet.h>

TreeItem::TreeItem(QString aName, TreeItem *aParentItem, TreeItemType aItemType)
{
    mParentItem = aParentItem;
    mItemName = aName;
    mItemType = aItemType;
}

TreeItem::TreeItem(quint64 aObjectID, TreeItem *aParentItem, TreeItem::TreeItemType aItemType)
{
    mItemType = aItemType;
    mParentItem = aParentItem;
    ProjectNamedObject *namedObject;
    if(aItemType == Class)
    {
        namedObject = ProjectModel::getInstance().getNetClassByID(aObjectID);
    }
    else if(aItemType == Sort)
    {
        namedObject = ProjectModel::getInstance().getSortByID(aObjectID);
    }
    else if(aItemType == ObjectNet)
    {
        namedObject = ProjectModel::getInstance()
                    .getNetClassByID(mParentItem->getObjectID())
                    ->getObjectNetByID(aObjectID);
    }
    if(namedObject)
    {
        mItemName = namedObject->name();
        connect(namedObject, SIGNAL(nameChanged(QString)), this, SLOT(nameChanged(QString)));
    }
    mObjectID = aObjectID;
}

TreeItem::~TreeItem()
{
    qDeleteAll(mChildItems);
}

void TreeItem::appendChild(TreeItem *aChild)
{
    mChildItems.append(aChild);
}

TreeItem *TreeItem::child(int aRow) const
{
    return mChildItems.value(aRow);
}

int TreeItem::childCount() const
{
    return mChildItems.count();
}

int TreeItem::dataLength() const
{
    return 1;
}

QVariant TreeItem::getData(int ) const
{
    return mItemName;
}

int TreeItem::row() const
{
    if(mParentItem)
    {
        return mParentItem->mChildItems.indexOf(const_cast<TreeItem*>(this));
    }

    return 0;
}

TreeItem *TreeItem::getParentItem() const
{
    return mParentItem;
}

QVariant TreeItem::getItemType() const
{
    return QVariant(mItemType);
}

QVariant TreeItem::getImage() const
{
    switch(mItemType)
    {
        case SortContainer:
            return QIcon(":/icons/icons/element types.ico");
        case Sort:
            return QIcon(":/icons/icons/type.ico");
        case ObjectNetsContainer:
            return QIcon(":/icons/icons/net container.ico");
        case Class:
            return QIcon(":/icons/icons/net class.ico");
        case Axiom:
        case ObjectNet:
            return QIcon(":/icons/icons/net.ico");
        default:
            return QVariant();
    }
}

QVariant TreeItem::getTextColor() const
{
    if(mItemType == Sort)
    {
        return ProjectModel::getInstance().getSortByID(mObjectID)->color();
    }
    return QVariant();
}

quint64 TreeItem::getObjectID() const
{
    return mObjectID;
}

void TreeItem::setObjectID(const quint64 &objectID)
{
    mObjectID = objectID;
}

void TreeItem::nameChanged(const QString &aNewName)
{
    mItemName = aNewName;
}
