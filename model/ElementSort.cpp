#include "ElementSort.h"

#include <QDebug>

ElementSort::ElementSort(QString aName, QColor *aColor)
{
    qDebug() << "Sort " << aName;
    mName = aName;
    mColor = aColor;
}

QColor *ElementSort::getColor() const
{
    return mColor;
}

QString ElementSort::getName() const
{
    return mName;
}

void ElementSort::setName(QString aName)
{
    mName = aName;
}

