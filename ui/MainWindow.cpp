#include "ui_mainwindow.h"
#include <QFile>
#include <QDebug>
#include <QTreeView>
#include <QInputDialog>
#include <QFileDialog>
#include <QAbstractItemModel>

#include "MainWindow.h"

#include <ui/viewmodel/ProjectTreeModel.h>
#include <ui/viewmodel/PropertyWidget.h>
#include <ui/viewmodel/TreeItem.h>
#include <ui/redactor/ObjectNetRedactor.h>

#include <model/ProjectModel.h>
#include <model/Connection.h>
#include <model/ObjectNet.h>
#include <model/NetClass.h>
#include <model/Place.h>
#include <model/NonTerminalTransition.h>

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
    mRedactorGroup = new QActionGroup(parent);
    mRedactorGroup->addAction(ui->actionControlConnection);
    mRedactorGroup->addAction(ui->actionPlace);
    mRedactorGroup->addAction(ui->actionCoursor);
    mRedactorGroup->addAction(ui->actionTransition);
    mRedactorGroup->addAction(ui->actionRegularConnection);
    mRedactorGroup->addAction(ui->actionNonTerminalTransition);
    mRedactorGroup->setEnabled(false);
}

void MainWindow::setStretchFatrors()
{
    ui->graphicSplitter->setStretchFactor(0,17);
    mPropertyWidget = new PropertyWidget();
    ui->graphicSplitter->addWidget(mPropertyWidget);
    ui->graphicSplitter->setStretchFactor(1,1);
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
        updateTreeModel();
    }
}

void MainWindow::showContextMenuOnExistingItem(QModelIndex aModelIndex, QPoint aPoint)
{
    QMenu *menu = new QMenu(this);

    int itemType = aModelIndex.model()->data(aModelIndex, Qt::UserRole).toInt();
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
    ui->treeView->update(aModelIndex);
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

void MainWindow::showProperties(const QModelIndex &aModelIndex)
{
    int itemType = aModelIndex.model()->data(aModelIndex, Qt::UserRole).toInt();
    if(itemType == TreeItem::Sort
            || itemType == TreeItem::Class
            || itemType == TreeItem::ObjectNet)
    {
        quint64 id = aModelIndex.model()->data(aModelIndex, Qt::UserRole + 1).toULongLong();
        if(itemType == TreeItem::ObjectNet)
        {
            quint64 parentID = aModelIndex.model()->data(aModelIndex.parent(), Qt::UserRole + 1).toULongLong();
            ObjectNet *net = ProjectModel::getInstance().getNetClassByID(parentID)->getObjectNetByID(id);
            mPropertyWidget->setProjectObject(net);
            mNetRedactor->setObjectNet(net);
            mRedactorGroup->setEnabled(true);
        }
        else
        {
            mRedactorGroup->setEnabled(false);
            mNetRedactor->clear();
            if(itemType == TreeItem::Sort)
            {
                mPropertyWidget->setProjectObject(ProjectModel::getInstance().getSortByID(id));
            }
            else if(itemType == TreeItem::Class)
            {
                mPropertyWidget->setProjectObject(ProjectModel::getInstance().getNetClassByID(id));
            }
        }
    }
    else
    {
        mRedactorGroup->setEnabled(false);
        mPropertyWidget->clear();
        mNetRedactor->clear();
    }
    ui->treeView->update(aModelIndex);
}

void MainWindow::placeSelected(const quint64 &aClassID, const quint64 &aNetID, const quint64 &aObjectID)
{
    Place *place = ProjectModel::getInstance().getNetClassByID(aClassID)->getPlaceByID(aObjectID, aNetID);
    mPropertyWidget->setProjectObject(place);
}

void MainWindow::transitionSelected(const quint64 &aClassID, const quint64 &aNetID, const quint64 &aObjectID)
{
    TerminalTransition *transition = ProjectModel::getInstance()
                                     .getNetClassByID(aClassID)
                                     ->getObjectNetByID(aNetID)
                                     ->getTransitionByID(aObjectID);
    mPropertyWidget->setProjectObject(transition);
}

void MainWindow::connectionSelected(const quint64 &aClassID, const quint64 &aNetID, const quint64 &aObjectID)
{
    Connection *connection = ProjectModel::getInstance()
                             .getNetClassByID(aClassID)
                             ->getObjectNetByID(aNetID)
                             ->getConnectionByID(aObjectID);
    mPropertyWidget->setProjectObject(connection);
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
    connect(ui->treeView->selectionModel(), SIGNAL(currentChanged(QModelIndex,QModelIndex)), this, SLOT(showProperties(QModelIndex)));
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
    connect(mNetRedactor, SIGNAL(placeSelected(quint64,quint64,quint64)), this, SLOT(placeSelected(quint64,quint64,quint64)));
    connect(mNetRedactor, SIGNAL(transitionSelected(quint64,quint64,quint64)), this, SLOT(transitionSelected(quint64,quint64,quint64)));
    connect(mNetRedactor, SIGNAL(connectionSelected(quint64,quint64,quint64)), this, SLOT(connectionSelected(quint64,quint64,quint64)));

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
