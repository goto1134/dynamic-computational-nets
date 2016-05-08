#ifndef NETCLASS_H
#define NETCLASS_H

#include <QObject>
#include <QMap>

#include "ObjectNet.h"

class NetClass
{
    public:
        explicit NetClass(QString aName);
        QString getName() const;
        void setName(const QString &aName);
        bool createObjectNet(QString aNetName);

    private:
        QString mName;
        QMap<QString, ObjectNet*> mObjectNets;

};

#endif // NETCLASS_H
