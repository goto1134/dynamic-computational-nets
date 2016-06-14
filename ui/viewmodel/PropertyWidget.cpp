#include "PropertyWidget.h"
#include "DataWidget.h"

#include <QErrorMessage>
#include <QLineEdit>
#include <QVBoxLayout>
#include "../../model/ElementSort.h"
#include "../../model/NetClass.h"
#include "../../model/ObjectNet.h"
#include "../../model/ProjectObject.h"
#include "../../model/ProjectModel.h"



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
    mLayout->addWidget(new DataWidget("name", mNameLE));
}

void PropertyWidget::addSortColorData()
{
    ElementSort *sort = static_cast<ElementSort *>(mObject);
    QColor color = sort->color();
    mRedSlider = getColorSlider(color.red());
    mLayout->addWidget(new DataWidget(tr("Red"), mRedSlider));
    mGreenSlider = getColorSlider(color.green());
    mLayout->addWidget(new DataWidget(tr("Green"), mGreenSlider));
    mBlueSlider = getColorSlider(color.blue());
    mLayout->addWidget(new DataWidget(tr("Blue"), mBlueSlider));
}

void PropertyWidget::updateData()
{
    clear();
    ProjectObject::Type type = mObject->type();
    QLineEdit *idLE = new QLineEdit(QString::number(mObject->ID()));
    idLE->setEnabled(false);
    QLineEdit *typeLE = new QLineEdit(QString::number(type));
    typeLE->setEnabled(false);
    mLayout->addWidget(new DataWidget("ID", idLE));
    mLayout->addWidget(new DataWidget("type", typeLE));

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
    }

    mApplyButton = new QPushButton(tr("Apply"));
    mCanselButton = new QPushButton(tr("Cansel"));
    connect(mApplyButton, SIGNAL(pressed()), this, SLOT(apply()));
    connect(mCanselButton, SIGNAL(pressed()), this, SLOT(cansel()));
    mLayout->addWidget(mApplyButton);
    mLayout->addWidget(mCanselButton);
    update();
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
    }

}

void PropertyWidget::cansel()
{
    updateData();
}

