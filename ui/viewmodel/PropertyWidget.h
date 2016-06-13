#ifndef PROPERTYWIDGET_H
#define PROPERTYWIDGET_H

#include <QObject>
#include <QWidget>
#include "../../model/ProjectObject.h"

class QLayout;
class QVBoxLayout;

class PropertyWidget : public QWidget
{
    public:
        PropertyWidget();
        void setProjectObject(ProjectObject* aObject);
        void clear();
    private:
        void remove(QLayout* layout);
        ProjectObject* mObject;
        QVBoxLayout *mLayout;
        void addTypeProperties();
    signals:

    public slots:
};

#endif // PROPERTYWIDGET_H
