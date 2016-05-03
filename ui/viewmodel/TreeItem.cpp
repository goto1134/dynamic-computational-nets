#include "TreeItem.h"
#include <QtAlgorithms>

TreeItem::TreeItem(const QList<QVariant> &aData, TreeItem *aParentItem)
{
    mParentItem = aParentItem;
    mItemData = aData;
}

TreeItem::~TreeItem()
{
    qDeleteAll(mChildItems);
}

void TreeItem::appendChild(TreeItem *aChild)
{
    mChildItems.append(aChild);
}

TreeItem *TreeItem::child(int aRow)
{
    return mChildItems.value(aRow);
}

int TreeItem::childCount() const
{
    return mChildItems.count();
}

int TreeItem::dataLength() const
{
    mItemData.count();
}

QVariant TreeItem::getData(int aIndex) const
{
    return mItemData.value(aIndex);
}

int TreeItem::row() const
{
    if(mParentItem)
    {
        return mParentItem->mChildItems.indexOf(const_cast<TreeItem*>(this));
    }

    return 0;
}

TreeItem *TreeItem::getParentItem()
{
    return mParentItem;
}
