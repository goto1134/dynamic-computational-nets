#ifndef CONNECTION_H
#define CONNECTION_H

#include <model/base/ProjectObject.h>

class ObjectNet;

const QString CONNECTION_LABEL = "connection";

class Connection : public ProjectObject
{
        Q_OBJECT
    public:
        enum ConnectionVariant
        {
            Control,
            InputTerminal,
            OutputTerminal,
            InputNonTerminal,
            OutputNonTerminal
        };

        explicit Connection(const quint64 &aID, 
                            const quint64 &aStartID, 
                            const quint64 &aEndID, 
                            const ConnectionVariant &aType,
                            ObjectNet *aNet = 0);
        explicit Connection(const quint64 &aID);
        explicit Connection(QXmlStreamReader *aInputStream, ObjectNet *aNet);

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

        ConnectionVariant connectionType() const;
        void switchControl();


    private:
        quint64 mStartID;
        quint64 mEndID;
        quint32 mResources;
        quint32 mTime;
        ConnectionVariant mConnectionType;
        ObjectNet *mNet;
    signals:
        void dataChanged();
    public slots:
};

#endif // CONNECTION_H
