#include "ObjectNet.h"

ObjectNet::ObjectNet(QString aName)
{
    mName = aName;
}


QString ObjectNet::getName() const
{
    return mName;
}

void ObjectNet::setName(const QString &aName)
{
    mName = aName;
}


