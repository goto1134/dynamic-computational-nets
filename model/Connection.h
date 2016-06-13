#ifndef CONNECTION_H
#define CONNECTION_H

#include "ProjectObject.h"

const QString CONNECTION_LABEL = "connection";

class Connection : public ProjectObject
{
    public:
        explicit Connection(const quint64 &aID);
        explicit Connection(QXmlStreamReader *aInputStream);

        void load(QXmlStreamReader *aInputStream);
        void save(QXmlStreamWriter *aOutputStream) const;

        quint64 startID() const;
        void setStartID(const quint64 &startID);

        quint64 endID() const;
        void setEndID(const quint64 &endID);

        int resources() const;
        void setResources(int resources);

        int time() const;
        void setTime(int time);

        bool isControl() const;
        void setIsControl(bool isControl);

    private:
        quint64 mStartID;
        quint64 mEndID;
        quint32 mResources;
        quint32 mTime;
        bool mIsControl;
    signals:

    public slots:
};

#endif // CONNECTION_H
