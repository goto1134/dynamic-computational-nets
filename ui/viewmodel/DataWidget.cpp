#include "DataWidget.h"

#include <QCheckBox>
#include <QComboBox>
#include <QSlider>

DataWidget::DataWidget(const QString &aText, QWidget *aValueWidget, QWidget *parent)
    : QWidget(parent)
{
    mLayout = new QHBoxLayout;
    setLayout(mLayout);
    mLayout->addWidget(new QLabel(aText));
    mLayout->addWidget(aValueWidget);
}
