#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class ProjectTreeModel;
class ObjectNetRedactor;
class PropertyTableModel;

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
        void showContextMenuOnExistingItem(QModelIndex modelIndex, QPoint aPoint);
        
        void createNetRedactor();

    private slots:
        void createNewProject();
        void showContextMenu(QPoint aPoint);
        void addSort();
        void addNetClass();
        void addObjectNet();
        void saveDVS();
};

#endif // MAINWINDOW_H
