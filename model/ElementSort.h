#ifndef ELEMENTSORT_H
#define ELEMENTSORT_H
#include <QString>
#include <QColor>
#include <QVariant>

class ElementSort
{
    public:
        ElementSort(QString aName, QColor *aColor = new QColor(Qt::black));
        QColor *getColor() const;
        QString getName() const;

        void setName(QString aName);
    private:
        QString mName;
        QColor *mColor;
//    signals:

//    public slots:
};

#endif // ELEMENTSORT_H
