#ifndef TREEITEM_H
#define TREEITEM_H

#include <QVariant>
#include <QObject>
#include <QList>

class TreeItem : public QObject
{
        Q_OBJECT
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
        explicit TreeItem(quint64 aObjectID, TreeItem *aParentItem, TreeItemType aItemType);
        ~TreeItem();

        void appendChild(TreeItem *aChild);

        TreeItem *child(int aRow) const;
        int childCount() const;
        int dataLength() const;
        QVariant getData(int aIndex) const;
        int row() const;
        TreeItem *getParentItem() const;
        QVariant getItemType() const;
        QVariant getImage() const;
        QVariant getTextColor() const;

        quint64 getObjectID() const;
        void setObjectID(const quint64 &objectID);

    private:
        QVariant mItemName;
        QList<TreeItem*> mChildItems;
        TreeItemType mItemType;
        TreeItem *mParentItem;
        quint64 mObjectID;
    private slots:
        void nameChanged(const QString &aNewName);
};

#endif // TREEITEM_H
