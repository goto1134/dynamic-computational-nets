#include "ProjectTreeModel.h"



ProjectTreeModel::ProjectTreeModel(const QString &aData, QObject *aParent):
    QAbstractItemModel(aParent)
{
    QList<QVariant> rootData;
    rootData << "Title" << "Summary";
    mRootItem = new TreeItem(rootData);
    setupModelData(aData.split(QString("\n")), mRootItem);
}

ProjectTreeModel::~ProjectTreeModel()
{
    delete mRootItem;
}

QVariant ProjectTreeModel::data(const QModelIndex &aIndex, int aRole) const Q_DECL_OVERRIDE
{
    if (!aIndex.isValid())
    {
        return QVariant();
    }

    if (aRole != Qt::DisplayRole)
    {
        return QVariant();
    }

    TreeItem *item = static_cast<TreeItem*>(aIndex.internalPointer());

    return item->getData(aIndex.column());
}

Qt::ItemFlags ProjectTreeModel::flags(const QModelIndex &aIndex) const Q_DECL_OVERRIDE
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



QModelIndex ProjectTreeModel::index(int aRow, int aColumn, const QModelIndex &aParent) const Q_DECL_OVERRIDE
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

void ProjectTreeModel::setupModelData(const QStringList &aLines, TreeItem *aParent)
{
    QList<TreeItem*> parents;
    QList<int> indentations;
    parents << aParent;
    indentations << 0;

    int number = 0;

    while (number < aLines.count())
    {
        int position = 0;
        while (position < aLines[number].length())
        {
            if (aLines[number].mid(position, 1) != " ")
            {
                break;
            }
            position++;
        }

        QString lineData = aLines[number].mid(position).trimmed();

        if (!lineData.isEmpty())
        {
            // Read the column data from the rest of the line.
            QStringList columnStrings = lineData.split("\t", QString::SkipEmptyParts);
            QList<QVariant> columnData;
            for (int column = 0; column < columnStrings.count(); ++column)
            {
                columnData << columnStrings[column];
            }

            if (position > indentations.last())
            {
                // The last child of the current parent is now the new parent
                // unless the current parent has no children.

                if (parents.last()->childCount() > 0)
                {
                    parents << parents.last()->child(parents.last()->childCount()-1);
                    indentations << position;
                }
            } else
            {
                while (position < indentations.last() && parents.count() > 0)
                {
                    parents.pop_back();
                    indentations.pop_back();
                }
            }

            // Append a new item to the current parent's list of children.
            parents.last()->appendChild(new TreeItem(columnData, parents.last()));
        }

        ++number;
    }
}


