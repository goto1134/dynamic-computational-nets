#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class ProjectTreeModel;
class ObjectNetRedactor;
class PropertyTableModel;
class ProjectModel;
class QModelIndex;
class PropertyWidget;
class QActionGroup;

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
        Q_OBJECT

    public:
        explicit MainWindow(QWidget *parent = 0);
        ~MainWindow();

    private:
        void setActionGroups(QWidget *parent);
        void setStretchFatrors();

        Ui::MainWindow *ui;

        ObjectNetRedactor *mNetRedactor;
        ProjectTreeModel *mTreeModel;
        PropertyWidget *mPropertyWidget;
        QActionGroup *mRedactorGroup;

        void showContextMenuOnExistingItem(QModelIndex aModelIndex, QPoint aPoint);
        void createNetRedactor();
        void updateTreeModel();
    public slots:
        void showProperties(const QModelIndex &aModelIndex);
    private slots:
        void placeSelected(const quint64 &aClassID, const quint64 &aNetID, const quint64 &aObjectID);
        void transitionSelected(const quint64 &aClassID, const quint64 &aNetID, const quint64 &aObjectID);
        void connectionSelected(const quint64 &aClassID, const quint64 &aNetID, const quint64 &aObjectID);
        void createNewProject();
        void showContextMenu(QPoint aPoint);
        void addSort();
        void addNetClass();
        void addObjectNet();
        void saveDVS();
        void loadProject();
};

#endif // MAINWINDOW_H
