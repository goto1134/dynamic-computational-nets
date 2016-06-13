#include "TreeItem.h"
#include <QtAlgorithms>
#include <QIcon>

#include "../../model/ProjectModel.h"

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
    if(aItemType == Class)
    {
        mItemName = ProjectModel::getInstance().getNetClassByID(aObjectID)->name();
    }
    else if(aItemType == Sort)
    {
        mItemName = ProjectModel::getInstance().getSortByID(aObjectID)->name();
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

void TreeItem::setItemName(QString aName)
{
    mItemName = aName;
}
