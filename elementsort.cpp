#include "elementsort.h"

ElementSort::ElementSort()
{
    mName="";
}

QString ElementSort::getName()
{
    return mName;
}

void ElementSort::setName(QString aName)
{
    mName = aName;
}

