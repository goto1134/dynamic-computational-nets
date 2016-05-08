#ifndef TREEITEM_H
#define TREEITEM_H

#include <QVariant>
#include <QList>

class TreeItem
{
    public:
        enum TreeItemType
        {
            Service = 0,
            SortContainer = 1,
            Sort = 2,
            ObjectNetsContainer = 3,
            Class = 4,
            ObjectNet = 5,
            Axiom = 6
        };


        explicit TreeItem(QString aName, TreeItem *aParentItem = 0, TreeItemType aItemType = Service);
        ~TreeItem();

        void appendChild(TreeItem *aChild);

        TreeItem *child(int aRow) const;
        int childCount() const;
        int dataLength() const;
        QVariant getData(int aIndex) const;
        int row() const;
        TreeItem *getParentItem() const;
        QVariant getItemType() const;

        void setItemName(QString aName);

    private:
        QVariant mItemName;
        QList<TreeItem*> mChildItems;
        QVariant mItemType;
        TreeItem *mParentItem;
};

#endif // TREEITEM_H
