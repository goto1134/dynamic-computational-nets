#ifndef PROPERTYWIDGET_H
#define PROPERTYWIDGET_H

#include <QLineEdit>
#include <QObject>
#include <QPushButton>
#include <QSlider>
#include <QWidget>

class ProjectObject;
class QLayout;
class QVBoxLayout;

class PropertyWidget : public QWidget
{
        Q_OBJECT
    public:
        PropertyWidget();
        void setProjectObject(ProjectObject* aObject);
        void clear();
    private:
        void remove(QLayout* layout);
        void addTypeProperties();
        void updateData();
        ProjectObject* mObject;

        QVBoxLayout *mLayout;

        //Имя
        QLineEdit *mNameLE;
        //Цвета
        QSlider *mRedSlider;
        QSlider *mGreenSlider;
        QSlider *mBlueSlider;
        QPushButton *mApplyButton;
        QPushButton *mCanselButton;


        void applyNamedObjectDataChanged();
        QSlider * getColorSlider(const short &aValue);

        void addNamedObjectData();

        void addSortColorData();

    signals:

    public slots:
        void apply();
        void cansel();
};

#endif // PROPERTYWIDGET_H
