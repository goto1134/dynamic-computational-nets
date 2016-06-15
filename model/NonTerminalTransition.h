#ifndef NONTERMINALTRANSITION_H
#define NONTERMINALTRANSITION_H

#include "TerminalTransition.h"

const QString NON_TERMINAL_TRANSITION = "non_terminal_transition";

class NonTerminalTransition : public TerminalTransition
{
        Q_OBJECT
    public:
        explicit NonTerminalTransition(const quint64 &aID, const QPointF &aPoint);
        explicit NonTerminalTransition(QXmlStreamReader *aInputStream);

        quint64 netID() const;
        void setNetID(const quint64 &netID);
        void setNetID(const quint64 &classID, const quint64 &netID);


        void load(QXmlStreamReader *aInputStream);
        void save(QXmlStreamWriter *aOutputStream) const;

        quint64 classID() const;
    private:
        quint64 mNetID;
        quint64 mClassID;

    signals:
        netChanged(const quint64 &aClassID, const quint64 &aNetID);
    public slots:
};

#endif // NONTERMINALTRANSITION_H
