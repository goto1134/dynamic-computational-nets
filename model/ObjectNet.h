#ifndef OBJECTNET_H
#define OBJECTNET_H

#include <QObject>

class ObjectNet
{
    public:
        explicit ObjectNet(QString aName);
        QString getName() const;
        void setName(const QString &aName);

    private:
        QString mName;
};

#endif // OBJECTNET_H
