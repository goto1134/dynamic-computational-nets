#ifndef PROPERTYWIDGET_H
#define PROPERTYWIDGET_H

#include <QCheckBox>
#include <QComboBox>
#include <QLineEdit>
#include <QPushButton>
#include <QSlider>
#include <QSpinBox>
#include <QWidget>

class QLayout;
class QVBoxLayout;
class ProjectObject;

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
        //Количество ресурсов
        QSpinBox *mResourceNumber;
        QSpinBox *mTimeValue;

        //Кнопки
        QPushButton *mApplyButton;
        QPushButton *mCanselButton;

        QCheckBox *mControlCheckBox;

        //Сорт позиции
        QComboBox *mSortBox;

        //Выбор класса
        QComboBox *mClassBox;
        QComboBox *mNetBox;


        void applyNamedObjectDataChanged();
        QSlider * getColorSlider(const short &aValue);

        void addNamedObjectData();

        void addSortColorData();

        void applySortSettings();

        QSpinBox * getSpinBoxWithValue(int value);

        void addNetClassData();

        void applyNetClassDataChanged();
        
        QLineEdit *getLineEdit(const bool &aEditable, const QString &aValue);
        
        void addPlaceSortsData();

        void applyPlaceDataChanged();

        void addNTTransitionData();

        void aplyNTTransitionData();

    signals:

    private slots:
        void apply();
        void cansel();
        void addNewClassData(int aClassIndex);
};

#endif // PROPERTYWIDGET_H
