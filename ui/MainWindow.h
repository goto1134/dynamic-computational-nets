#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "viewmodel/ProjectTreeModel.h"
#include "redactor/objectnetredactor.h"

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
        
    private slots:
        void createNewProject();
        void showContextMenu(QPoint aPoint);
        void addSort();
        void addNetClass();
        void addObjectNet();
};

#endif // MAINWINDOW_H
