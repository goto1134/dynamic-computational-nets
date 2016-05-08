#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "viewmodel/ProjectTreeModel.h"

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

        ProjectTreeModel *mTreeModel;
    private slots:
        void createNewProject();
        void showContextMenu(QPoint aPoint);
        void createProject();
        void addSort();
        void addNetClass();
        void addObjectNet();
};

#endif // MAINWINDOW_H
