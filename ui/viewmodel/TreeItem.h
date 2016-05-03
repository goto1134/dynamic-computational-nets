#ifndef TREEITEM_H
#define TREEITEM_H

#include <QVariant>
#include <QList>

class TreeItem
{
    public:
        explicit TreeItem(const QList<QVariant> &aData, TreeItem *aParentItem = 0);
        ~TreeItem();

        void appendChild(TreeItem *aChild);

        TreeItem *child(int aRow);
        int childCount() const;
        int dataLength() const;
        QVariant getData(int aIndex) const;
        int row() const;
        TreeItem *getParentItem();

    private:
        QList<TreeItem*> mChildItems;
        QList<QVariant> mItemData;
        TreeItem *mParentItem;
};

#endif // TREEITEM_H
