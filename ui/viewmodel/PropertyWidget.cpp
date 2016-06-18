#include "PropertyWidget.h"
#include "DataWidget.h"

#include <QErrorMessage>
#include <QVBoxLayout>

#include <model/Connection.h>
#include <model/ElementSort.h>
#include <model/NonTerminalTransition.h>
#include <model/Place.h>
#include <model/ProjectModel.h>
#include <model/NetClass.h>
#include <model/ObjectNet.h>

#include <model/base/ProjectObject.h>

PropertyWidget::PropertyWidget()
{
    mLayout = new QVBoxLayout(this);
    setLayout(mLayout);
    mLayout->setAlignment(Qt::AlignTop);
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy ::Expanding);
}

void PropertyWidget::setProjectObject(ProjectObject *aObject)
{
    mObject = aObject;
    updateData();
}

void PropertyWidget::clear()
{
    remove(mLayout);
    update();
}

void PropertyWidget::remove(QLayout* layout)
{
    QLayoutItem* child;
    while(layout->count()!=0)
    {
        child = layout->takeAt(0);
        if(child->layout() != 0)
        {
            remove(child->layout());
        }
        else if(child->widget() != 0)
        {
            delete child->widget();
        }
        delete child;
    }
}

QSlider * PropertyWidget::getColorSlider(const short &aValue)
{
    QSlider *slider = new QSlider(Qt::Horizontal);
    slider->setMinimum(0);
    slider->setMaximum(255);
    slider->setValue(aValue);
    return slider;
}

void PropertyWidget::addNamedObjectData()
{
    ProjectNamedObject *namedObject = static_cast<ProjectNamedObject *>(mObject);
    mNameLE = new QLineEdit(namedObject->name());
    addWidget("name", mNameLE);
}

void PropertyWidget::addSortColorData()
{
    ElementSort *sort = static_cast<ElementSort *>(mObject);
    QColor color = sort->color();

    mRedSlider = getColorSlider(color.red());
    addWidget(tr("Red"), mRedSlider);

    mGreenSlider = getColorSlider(color.green());
    addWidget(tr("Green"), mGreenSlider);

    mBlueSlider = getColorSlider(color.blue());
    addWidget(tr("Blue"), mBlueSlider);

}

QSpinBox *PropertyWidget::getSpinBoxWithValue(int value)
{
    QSpinBox *spinBox = new QSpinBox();
    spinBox->setMinimum(0);
    spinBox->setValue(value);
    return spinBox;
}

QLineEdit *PropertyWidget::getLineEdit(const bool &aEditable,const QString &aValue)
{
    QLineEdit *lineEdit = new QLineEdit(aValue);
    lineEdit->setEnabled(aEditable);
    return lineEdit;
}

void PropertyWidget::addPlaceSortsData()
{
    Place *place = static_cast<Place *>(mObject);

    QLineEdit *typeLE = getLineEdit(false, QString::number(place->getPlaceType()));
    addWidget(tr("Place type"), typeLE);
    mResourceNumber = getSpinBoxWithValue(place->resourceNumber());
    if(place->getPlaceType() != Place::Common)
    {
        mResourceNumber->setEnabled(false);
    }
    addWidget(tr("Resources"), mResourceNumber);
    mSortBox = new QComboBox();
    foreach (quint64 sortID, ProjectModel::getInstance().getSortsIDs())
    {
        mSortBox->addItem(QIcon(),ProjectModel::getInstance().getSortByID(sortID)->name(), QVariant(sortID));
    }
    mSortBox->setCurrentIndex(mSortBox->findData(QVariant(place->sortID())));
    addWidget(tr("Sort"), mSortBox);
}

void PropertyWidget::addNTTransitionData()
{
    NonTerminalTransition *transition = static_cast<NonTerminalTransition *>(mObject);
    mClassBox = new QComboBox();
    mNetBox = new QComboBox();
    connect(mClassBox, SIGNAL(currentIndexChanged(int)), this, SLOT(addNewClassData(int)));
    foreach (NetClass *netClass, ProjectModel::getInstance().getNetClasses())
    {
        mClassBox->addItem(QIcon(),netClass->name(),QVariant(netClass->ID()));
    }
    if(transition->classID() != 0)
    {
        mClassBox->setCurrentIndex(mClassBox->findData(QVariant(transition->classID())));
    }
    if(transition->netID() != 0)
    {
        mNetBox->setCurrentIndex(mNetBox->findData(QVariant(transition->netID())));
    }
    addWidget(tr("Class"), mClassBox);
    addWidget(tr("Net"), mNetBox);
    mTimeValue = getSpinBoxWithValue(transition->time());
    addWidget(tr("Time"), mTimeValue);
}

void PropertyWidget::updateData()
{
    clear();
    ProjectObject::Type type = mObject->type();
    QLineEdit *idLE = getLineEdit(false, QString::number(mObject->ID()));
    addWidget(tr("ID"), idLE);
    QLineEdit *typeLE = getLineEdit(false, QString::number(type));
    addWidget(tr("type"), typeLE);

    //имя ProjectNamedObject
    if(type == ProjectObject::Sort
            || type == ProjectObject::NetClass
            || type == ProjectObject::Net)
    {
        addNamedObjectData();
        if(type == ProjectObject::Sort)
        {
            addSortColorData();
        }
        else if(type == ProjectObject::NetClass)
        {
            addNetClassData();
        }
        else if(type == ProjectObject::Net)
        {
            ObjectNet *net = static_cast<ObjectNet *>(mObject);
            addWidget(tr("Class ID"), getLineEdit(false, QString::number(net->netClassID())));
        }
    }
    else
    {
        if(type == ProjectObject::PlaceType)
        {
            addPlaceSortsData();
        }
        else if(type == ProjectObject::NTTransition)
        {
            addNTTransitionData();
        }
        else if(type == ProjectObject::ConnectionType)
        {
            Connection *connection = static_cast<Connection *>(mObject);
            mResourceNumber = getSpinBoxWithValue(connection->resources());
            addWidget(tr("Resources"), mResourceNumber);
            mTimeValue = getSpinBoxWithValue(connection->time());
            addWidget(tr("Time"), mTimeValue);
            Connection::ConnectionVariant conType = connection->connectionType();
            if(conType == Connection::InputNonTerminal
                    || conType == Connection::Control)
            {
                mControlCheckBox = new QCheckBox();
                mControlCheckBox->setChecked(conType == Connection::Control);
                addWidget(tr("Is control"), mControlCheckBox);
            }
        }
    }

    mApplyButton = new QPushButton(tr("Apply"));
    mCanselButton = new QPushButton(tr("Cansel"));
    connect(mApplyButton, SIGNAL(pressed()), this, SLOT(apply()));
    connect(mCanselButton, SIGNAL(pressed()), this, SLOT(cansel()));
    mLayout->addWidget(mApplyButton);
    mLayout->addWidget(mCanselButton);
    update();
}

void PropertyWidget::addWidget(const QString &aText, QWidget *aWidget)
{
    mLayout->addWidget(new DataWidget(aText, aWidget));
}

void PropertyWidget::applyNamedObjectDataChanged()
{
    ProjectObject::Type type = mObject->type();
    ProjectNamedObject *namedObject = static_cast<ProjectNamedObject *>(mObject);
    if(namedObject->name() != mNameLE->text())
    {
        if(type == ProjectObject::Sort)
        {
            if(!ProjectModel::getInstance().setSortName(namedObject->ID(), mNameLE->text()))
            {
                QErrorMessage *error = new QErrorMessage(parentWidget());
                error->showMessage(tr("Sort with this name already exists"));
            }
        }
        else if(type == ProjectObject::NetClass)
        {
            if(!ProjectModel::getInstance().setNetClassName(namedObject->ID(), mNameLE->text()))
            {
                QErrorMessage *error = new QErrorMessage(parentWidget());
                error->showMessage(tr("Class with this name already exists"));
            }
        }
        else if(type == ProjectObject::Net)
        {
            ObjectNet *net = static_cast<ObjectNet *>(namedObject);
            if(!ProjectModel::getInstance().getNetClassByID(net->netClassID())->setNetName(net->ID(), mNameLE->text()))
            {
                QErrorMessage *error = new QErrorMessage(parentWidget());
                error->showMessage(tr("Net with this name already exists in this class"));
            }
        }
    }
}

void PropertyWidget::applySortSettings()
{
    ElementSort *sort = static_cast<ElementSort *>(mObject);
    QColor color = sort->color();
    QColor newColor = QColor(mRedSlider->value(), mGreenSlider->value(), mBlueSlider->value());
    if(color != newColor)
    {
        sort->setColor(newColor);
    }
}

void PropertyWidget::addNetClassData()
{
    NetClass* netClass = static_cast<NetClass *>(mObject);

    mInputSB = getSpinBoxWithValue(netClass->getInputPlaceNumber());
    addWidget(tr("Input places"), mInputSB);

    mOutputSB = getSpinBoxWithValue(netClass->getOutputPlaceNumber());
    addWidget(tr("Output places"), mOutputSB);
}

void PropertyWidget::applyNetClassDataChanged()
{
    NetClass* netClass = static_cast<NetClass *>(mObject);
    int inputPlaces = mInputSB->value();
    if(inputPlaces != netClass->getInputPlaceNumber())
    {
        netClass->setInputPlaceNumber(inputPlaces);
    }

    int outputPlaces = mOutputSB->value();
    if(outputPlaces != netClass->getOutputPlaceNumber())
    {
        netClass->setOutputPlaceNumber(outputPlaces);
    }
}

void PropertyWidget::applyPlaceDataChanged()
{
    Place *place = static_cast<Place *>(mObject);
    quint64 sortID = mSortBox->currentData().toULongLong();
    if(place->sortID() != sortID)
    {
        place->setSortID(sortID);
    }
    quint64 resourceNumber = mResourceNumber->value();
    if(place->getPlaceType() == Place::Common
            && place->resourceNumber() != resourceNumber)
    {
        place->setResourceNumber(resourceNumber);
    }
}

void PropertyWidget::aplyNTTransitionData()
{
    NonTerminalTransition *transition = static_cast<NonTerminalTransition *>(mObject);
    quint64 classID = mClassBox->currentData().toULongLong();
    quint64 netID = mNetBox->currentData().toULongLong();
    if(classID != transition->classID())
    {
        transition->setNetID(classID, netID);
    }
    else if(netID != transition->netID())
    {
        transition->setNetID(netID);
    }
    int time = mTimeValue->value();
    if(transition->time() != time)
    {
        transition->setTime(time);
    }
}

void PropertyWidget::apply()
{
    ProjectObject::Type type = mObject->type();
    if(type == ProjectObject::Sort
            || type == ProjectObject::NetClass
            || type == ProjectObject::Net)
    {
        applyNamedObjectDataChanged();
        if(type == ProjectObject::Sort)
        {
            applySortSettings();
        }
        else if(type == ProjectObject::NetClass)
        {
            applyNetClassDataChanged();
        }
    }
    else
    {
        if(type == ProjectObject::PlaceType)
        {
            applyPlaceDataChanged();
        }
        else if(type == ProjectObject::NTTransition)
        {
            aplyNTTransitionData();
        }
        else if(type == ProjectObject::ConnectionType)
        {
            Connection *connection = static_cast<Connection *>(mObject);
            int resourceNumber = mResourceNumber->value();
            if(resourceNumber != connection->resources())
            {
                connection->setResources(resourceNumber);
            }
            int time = mTimeValue->value();
            if(time != connection->time())
            {
                connection->setTime(time);
            }
            Connection::ConnectionVariant conType = connection->connectionType();
            if(conType == Connection::InputNonTerminal
                    || conType == Connection::Control)
            {
                bool isControl = mControlCheckBox->isChecked();
                if(isControl)
                {
                    if(conType == Connection::InputNonTerminal)
                    {
                        connection->switchControl();
                    }
                }
                else if(conType == Connection::Control)
                {
                    connection->switchControl();
                }
            }
        }
    }

}

void PropertyWidget::cansel()
{
    updateData();
}

void PropertyWidget::addNewClassData(int aClassIndex)
{
    if(aClassIndex > -1)
    {
        mNetBox->clear();
        quint64 classID = mClassBox->itemData(aClassIndex).toULongLong();
        foreach(ObjectNet *net, ProjectModel::getInstance().getNetClassByID(classID)->getObjectNets())
        {
            mNetBox->addItem(QIcon(),net->name(), QVariant(net->ID()));
        }
    }
}

