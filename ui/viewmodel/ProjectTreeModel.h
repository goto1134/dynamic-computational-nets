#ifndef PROJECTTREEMODEL_H
#define PROJECTTREEMODEL_H

#include <QObject>
#include <QAbstractItemModel>

#include "TreeItem.h"
#include "model/ElementSort.h"
#include "model/NetClass.h"

class ProjectTreeModel : public QAbstractItemModel
{
        Q_OBJECT
    public:
        explicit ProjectTreeModel(QObject *aParent = 0);
        ~ProjectTreeModel();
        QVariant data(const QModelIndex &aIndex, int aRole) const Q_DECL_OVERRIDE;
        Qt::ItemFlags flags(const QModelIndex &aIndex) const Q_DECL_OVERRIDE;
        QVariant headerData(int aSection, Qt::Orientation aOrientation, int aRole = Qt::DisplayRole) const Q_DECL_OVERRIDE;
        QModelIndex index(int aRow, int aColumn, const QModelIndex &aParent = QModelIndex()) const Q_DECL_OVERRIDE;
        QModelIndex parent(const QModelIndex &aIndex) const Q_DECL_OVERRIDE;
        int rowCount(const QModelIndex &aParent = QModelIndex()) const Q_DECL_OVERRIDE;
        int columnCount(const QModelIndex &aParent = QModelIndex()) const Q_DECL_OVERRIDE;
        void updateSortsData(QList<QString> aSortNames);

        void addElementSort(QString aSortName);
        void addNetClass(QString aClassName);
        void addObjectNet(QModelIndex &aParent, QString aNetName);

    private:

        QMap<QString, ElementSort*> mElementSorts;
        QMap<QString, NetClass*> mNetClasses;
        TreeItem *mRootItem;
        TreeItem *mSortParent;
        TreeItem *mObjectNetsParent;
        TreeItem *mAxiom;
};

#endif // PROJECTTREEMODEL_H
