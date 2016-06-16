#ifndef CONNECTION_H
#define CONNECTION_H

#include <model/base/ProjectObject.h>

const QString CONNECTION_LABEL = "connection";

class Connection : public ProjectObject
{
        Q_OBJECT
    public:
        enum ConnectionVariant
        {
            Control,
            FromPlace,
            FromTransition
        };
        explicit Connection(const quint64 &aID, const quint64 &aStartID, const quint64 &aEndID, const ConnectionVariant &type);
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

        ConnectionVariant connectionType() const;

    private:
        quint64 mStartID;
        quint64 mEndID;
        quint32 mResources;
        quint32 mTime;
        ConnectionVariant mConnectionType;
        bool mIsControl;
    signals:
        dataChanged();
    public slots:
};

#endif // CONNECTION_H
