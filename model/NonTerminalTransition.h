#ifndef NONTERMINALTRANSITION_H
#define NONTERMINALTRANSITION_H

#include "TerminalTransition.h"

const QString NON_TERMINAL_TRANSITION = "non_terminal_transition";

class NonTerminalTransition : public TerminalTransition
{
    public:
        explicit NonTerminalTransition(const quint64 &aID);
        explicit NonTerminalTransition(QXmlStreamReader *aInputStream);

        quint64 netID() const;
        void setNetID(const quint64 &netID);

        void load(QXmlStreamReader *aInputStream);
        void save(QXmlStreamWriter *aOutputStream) const;

    private:
        quint64 mNetID;

    signals:

    public slots:
};

#endif // NONTERMINALTRANSITION_H
