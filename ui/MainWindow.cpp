#include "ui_mainwindow.h"
#include <QFile>
#include <QDebug>
#include <QTreeView>
#include <QInputDialog>
#include <QFileDialog>

#include "MainWindow.h"

#include "model/ProjectNamedObject.h"
#include "viewmodel/ProjectTreeModel.h"
#include "redactor/ObjectNetRedactor.h"
#include "model/ElementSort.h"
#include "model/Place.h"
#include "model/NetClass.h"
#include "model/Connection.h"
#include "model/TerminalTransition.h"
#include "model/NonTerminalTransition.h"
#include "model/ObjectNet.h"
#include "model/ProjectModel.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    setActionGroups(parent);
    setStretchFatrors();

    ui->treeView->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->treeView, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(showContextMenu(QPoint)));

    connect(ui->actionCreateDVS, SIGNAL(triggered()), this, SLOT(createNewProject()));
    connect(ui->actionLoadDVS, SIGNAL(triggered()), this, SLOT(loadProject()));

    connect(ui->actionSaveDVS, SIGNAL(triggered()), this, SLOT(saveDVS()));

    createNetRedactor();
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
    actionGroup->addAction(ui->actionNonTerminalTransition);
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
        ProjectModel::newProject(text);
        //        mTreeModel = new ProjectTreeModel(text);
        updateTreeModel();
    }
}

void MainWindow::showContextMenuOnExistingItem(QModelIndex modelIndex, QPoint aPoint)
{
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
        menu->addAction(new QAction("Menu is Not Configured for this item", this));
    }
    menu->popup(ui->treeView->viewport()->mapToGlobal(aPoint));
    ui->treeView->update(modelIndex);
}

void MainWindow::showContextMenu(QPoint aPoint)
{
    QModelIndex modelIndex = ui->treeView->indexAt(aPoint);
    qDebug() << modelIndex.column() << modelIndex.row() << modelIndex.parent();

    if(modelIndex.column() > -1 && modelIndex.row() > -1)
    {
        showContextMenuOnExistingItem(modelIndex, aPoint);
    }
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

void MainWindow::saveDVS()
{
    if(& ProjectModel::getInstance())
    {
        QString fileName = QFileDialog::getSaveFileName(this, "Save project", QDir::currentPath(), tr("DC net project  (*.dcn)"));
        QFile file( fileName );
        if ( file.open(QIODevice::ReadWrite) )
        {
            file.resize(0);
            QXmlStreamWriter stream( &file );
            ProjectModel &project = ProjectModel::getInstance();
            if(&project)
            {
                project.save(&stream);
            }
        }
        file.close();
    }

}

void MainWindow::updateTreeModel()
{
    mTreeModel = new ProjectTreeModel();
    ui->treeView->setModel(mTreeModel);
    ui->treeView->expandAll();
}

void MainWindow::loadProject()
{
    QString fileName = QFileDialog::getOpenFileName(this, "Open project", QDir::currentPath(), tr("DC net project  (*.dcn)"));
    QFile file( fileName );
    if(file.open(QIODevice::ReadOnly))
    {
        QXmlStreamReader stream (&file);
        ProjectModel &project = ProjectModel::newProject(&stream);
        updateTreeModel();
        qDebug() << "point";
        qDebug() << &project;
    }
    file.close();
}

void MainWindow::createNetRedactor()
{
    mNetRedactor = new ObjectNetRedactor(this);
    ui->graphicsView->setScene(mNetRedactor);
    ui->graphicsView->setRenderHint(QPainter::Antialiasing);
    connect(ui->actionCoursor, SIGNAL(triggered()), mNetRedactor, SLOT(setMouseTool()));
    connect(ui->actionPlace, SIGNAL(triggered()), mNetRedactor, SLOT(setPlaceTool()));
    connect(ui->actionTransition, SIGNAL(triggered()), mNetRedactor, SLOT(setTerminalTransitionTool()));
    connect(ui->actionNonTerminalTransition, SIGNAL(triggered()), mNetRedactor, SLOT(setNonTerminalTransitionTool()));
    connect(ui->actionRegularConnection, SIGNAL(triggered()), mNetRedactor, SLOT(setConnectionTool()));
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
