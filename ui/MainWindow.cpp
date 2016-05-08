#include "ui_mainwindow.h"
#include <QFile>
#include <QDebug>
#include <QTreeView>
#include <QInputDialog>

#include "MainWindow.h"
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    setActionGroups(parent);
    connect(ui->actionCreateDVS, SIGNAL(triggered()), this, SLOT(createProject()));
    setStretchFatrors();

    ui->treeView->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->treeView, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(showContextMenu(QPoint)));

    connect(ui->actionCreateDVS, SIGNAL(triggered()), this, SLOT(createNewProject()));
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

void MainWindow::createNewProject()
{
    bool ok;
    QString text = QInputDialog::getText(this,
                                 QString("Enter Project Name"),
                                 QString("Project Name:"),
                                 QLineEdit::Normal, "", &ok);
    if (ok && !text.isEmpty())
    {
        mTreeModel = new ProjectTreeModel(text);
        ui->treeView->setModel(mTreeModel);
        ui->treeView->expandAll();
    }
}

void MainWindow::showContextMenu(QPoint aPoint)
{
    QModelIndex modelIndex = ui->treeView->indexAt(aPoint);
    qDebug() << modelIndex.column() << modelIndex.row() << modelIndex.parent();

    QMenu *menu = new QMenu(this);

    int itemType = modelIndex.model()->data(modelIndex, Qt::UserRole).toInt();
    if(itemType == TreeItem::Sort)
    {
        menu->addAction("Rename");

    }
    else if(itemType == TreeItem::SortContainer)
    {
        menu->addAction("Add Sort", this, SLOT(addSort()));
    }
    else if(itemType == TreeItem::ObjectNetsContainer)
    {
        menu->addAction("Add Net Class", this, SLOT(addNetClass()));
    }
    else if(itemType == TreeItem::Class)
    {
        menu->addAction("Add Object Net", this, SLOT(addObjectNet()));
    }
    else
    {
        menu->addAction(new QAction("Action 1", this));
        menu->addAction(new QAction("Action 2", this));
        menu->addAction(new QAction("Action 3", this));
    }
    menu->popup(ui->treeView->viewport()->mapToGlobal(aPoint));
    ui->treeView->update(modelIndex);
}

void MainWindow::createProject()
{

}

void MainWindow::addSort()
{
    bool ok;
    QString text = QInputDialog::getText(this,
                                 QString("Add Sort"),
                                 QString("Sort Name:"),
                                 QLineEdit::Normal, "", &ok);
    if (ok && !text.isEmpty())
    {
        mTreeModel->addElementSort(text);
    }
}

void MainWindow::addNetClass()
{
    bool ok;
    QString text = QInputDialog::getText(this,
                                 QString("Add Net Class"),
                                 QString("Clss Name:"),
                                 QLineEdit::Normal, "", &ok);
    if (ok && !text.isEmpty())
    {
        mTreeModel->addNetClass(text);
    }

}

void MainWindow::addObjectNet()
{
    QModelIndex classIndex = ui->treeView->selectionModel()->currentIndex();
    bool ok;
    QString text = QInputDialog::getText(this,
                                 QString("Add Object Net"),
                                 QString("Net Name:"),
                                 QLineEdit::Normal, "", &ok);
    if (ok && !text.isEmpty())
    {
        mTreeModel->addObjectNet(classIndex, text);
    }
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
