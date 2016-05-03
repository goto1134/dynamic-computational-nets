#ifndef DCNTREEMODEL_H
#define DCNTREEMODEL_H

#include <QObject>
#include <QAbstractItemModel>

#include "TreeItem.h"

class ProjectTreeModel : public QAbstractItemModel
{
    public:
        explicit ProjectTreeModel(const QString &aData, QObject *aParent = 0);
        ~ProjectTreeModel();
        QVariant data(const QModelIndex &aIndex, int aRole) const Q_DECL_OVERRIDE;
        Qt::ItemFlags flags(const QModelIndex &aIndex) const Q_DECL_OVERRIDE;
        QVariant headerData(int aSection, Qt::Orientation aOrientation, int aRole = Qt::DisplayRole) const Q_DECL_OVERRIDE;
        QModelIndex index(int aRow, int aColumn, const QModelIndex &aParent = QModelIndex()) const Q_DECL_OVERRIDE;
        QModelIndex parent(const QModelIndex &aIndex) const Q_DECL_OVERRIDE;
        int rowCount(const QModelIndex &aParent = QModelIndex()) const Q_DECL_OVERRIDE;
        int columnCount(const QModelIndex &aParent = QModelIndex()) const Q_DECL_OVERRIDE;

       private:
           void setupModelData(const QStringList &aLines, TreeItem *aParent);

           TreeItem *mRootItem;
};

#endif // DCNTREEMODEL_H
