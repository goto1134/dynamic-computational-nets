#include "DataWidget.h"

#include <QCheckBox>
#include <QComboBox>
#include <QSlider>

DataWidget::DataWidget(const QString &aText, QWidget *aValueWidget, QWidget *parent)
    : QWidget(parent)
{
    mLayout = new QHBoxLayout;
    setLayout(mLayout);
    QLabel *label = new QLabel(aText);
    mLayout->addWidget(label, 1);
    mLayout->addWidget(aValueWidget,5);
}
