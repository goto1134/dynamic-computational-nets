#include "PropertyWidget.h"
#include "DataWidget.h"

#include <QLineEdit>
#include <QVBoxLayout>
#include "../../model/ElementSort.h"
#include "../../model/NetClass.h"
#include "../../model/ObjectNet.h"

PropertyWidget::PropertyWidget()
{
    mLayout = new QVBoxLayout(this);
    setLayout(mLayout);
    mLayout->setAlignment(Qt::AlignTop);
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy ::Expanding);
}

void PropertyWidget::setProjectObject(ProjectObject *aObject)
{
    clear();
    ProjectObject::Type type = aObject->type();
    QLineEdit *idLE = new QLineEdit(QString::number(aObject->ID()));
    idLE->setEnabled(false);
    QLineEdit *typeLE = new QLineEdit(QString::number(type));
    typeLE->setEnabled(false);
    mLayout->addWidget(new DataWidget("ID", idLE));
    mLayout->addWidget(new DataWidget("type", typeLE));

    if(type == ProjectObject::Sort
            || type == ProjectObject::NetClass
            || type == ProjectObject::Net)
    {
        ProjectNamedObject *namedObject = static_cast<ProjectNamedObject *>(aObject);
        QLineEdit *nameLE = new QLineEdit(namedObject->name());
        mLayout->addWidget(new DataWidget("name", nameLE));
    }
    update();
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

