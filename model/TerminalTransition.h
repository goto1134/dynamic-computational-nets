#ifndef TERMINALTRANSITION_H
#define TERMINALTRANSITION_H

#include "ProjectObject.h"

const QString TERMINAL_TRANSITION = "terminal_transition";

class TerminalTransition : public ProjectObject
{
    public:
        explicit TerminalTransition();
        explicit TerminalTransition(const quint64 &aID);
        explicit TerminalTransition(QXmlStreamReader *aInputStream);

        void load(QXmlStreamReader *aInputStream);
        void save(QXmlStreamWriter *aOutputStream) const;

        void addInputConnectionID(const quint64 &connectionID);
        void addOutputConnectionID(const quint64 &connectionID);
    protected:
        explicit TerminalTransition(const Type &aType, const quint64 &aID);
    private:
        QSet<quint64> mInputConnectionsIDs;
        QSet<quint64> mOutputConnectionsIDs;

        void saveConnectionIDs(QSet<quint64> source, QXmlStreamWriter *aOutputStream, QString name) const;
        void getConnectionsIDs(QSet<quint64> *aConnectionSet, QXmlStreamReader *aInputStream, QString name);
    signals:

    public slots:
};

#endif // TERMINALTRANSITION_H
