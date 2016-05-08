#include "TreeItem.h"
#include <QtAlgorithms>

//TreeItem::TreeItem(const QList<QVariant> &aData, TreeItem *aParentItem)
//{
//    mParentItem = aParentItem;
//    mItemData = aData;
//}

TreeItem::TreeItem(QString aName, TreeItem *aParentItem, TreeItemType aItemType)
{
    mParentItem = aParentItem;
    mItemName = aName;
    mItemType = aItemType;
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

QVariant TreeItem::getData(int aIndex) const
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
    return mItemType;
}

void TreeItem::setItemName(QString aName)
{
    mItemName = aName;
}
