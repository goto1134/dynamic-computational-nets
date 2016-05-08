#include "NetClass.h"

NetClass::NetClass(QString aName)
{
    mName = aName;
}


QString NetClass::getName() const
{
    return mName;
}

void NetClass::setName(const QString &aName)
{
    mName = aName;
}

bool NetClass::createObjectNet(QString aNetName)
{
    if(!mObjectNets.contains(aNetName))
    {
        mObjectNets.insert(aNetName, new ObjectNet(aNetName));
        return true;
    }
    return false;
}
