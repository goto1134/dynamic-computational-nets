#include "ui_mainwindow.h"
#include <QFile>

#include "MainWindow.h"
#include "viewmodel/ProjectTreeModel.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    setActionGroups(parent);
    setStretchFatrors();

    QFile file(":/default.txt");
    file.open(QIODevice::ReadOnly);

    ui->treeView->setModel(new ProjectTreeModel(file.readAll()));

    file.close();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setActionGroups(QWidget *parent)
{
    QActionGroup *actionGroup = new QActionGroup(parent);
    actionGroup->addAction(ui->actionControlConnection);
    actionGroup->addAction(ui->actionPlace);
    actionGroup->addAction(ui->actionCoursor);
    actionGroup->addAction(ui->actionTransition);
    actionGroup->addAction(ui->actionRegularConnection);
}

void MainWindow::setStretchFatrors()
{
    ui->graphicSplitter->setStretchFactor(0,5);
    ui->graphicSplitter->setStretchFactor(1,0);
    ui->projectSplitter->setStretchFactor(0,0);
    ui->projectSplitter->setStretchFactor(1,6);
}


//void addToolBarToWidget()
//{
    //    QToolBar* toolBar = new QToolBar();
    //    toolBar->addAction(ui->actionCoursor);
    //    toolBar->addAction(ui->actionPlace);
    //    QWidget* view = new QWidget();

    //    QVBoxLayout* vbox = new QVBoxLayout();
    //    vbox->addWidget(toolBar);
    //    vbox->addWidget(view);
    //    ui->widget->setLayout(vbox);
//}
