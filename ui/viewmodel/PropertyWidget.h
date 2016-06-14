#ifndef PROPERTYWIDGET_H
#define PROPERTYWIDGET_H

#include <QLineEdit>
#include <QObject>
#include <QPushButton>
#include <QSlider>
#include <QSpinBox>
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
        void addWidget(const QString &aText, QWidget * aWidget);
        ProjectObject* mObject;

        QVBoxLayout *mLayout;

        //Имя
        QLineEdit *mNameLE;
        //Цвета
        QSlider *mRedSlider;
        QSlider *mGreenSlider;
        QSlider *mBlueSlider;

        //Количества входных и выходных позиций
        QSpinBox *mInputSB;
        QSpinBox *mOutputSB;

        //Кнопки
        QPushButton *mApplyButton;
        QPushButton *mCanselButton;


        void applyNamedObjectDataChanged();
        QSlider * getColorSlider(const short &aValue);

        void addNamedObjectData();

        void addSortColorData();

        void applySortSettings();

        QSpinBox * getSpinBoxWithValue(int value);

        void addNetClassData();

        void applyNetClassDataChanged();
        
        QLineEdit *getLineEdit(const bool &aEditable, const QString &aValue);
        
    signals:

    public slots:
        void apply();
        void cansel();
};

#endif // PROPERTYWIDGET_H
