#ifndef ELEMENTSORT_H
#define ELEMENTSORT_H

#include <QtCore>
#include <QColor>
#include "ProjectNamedObject.h"

class ElementSort : public ProjectNamedObject
{
    public:
        explicit ElementSort(const QString &aName, const QColor &aColor = QColor(Qt::black));
        explicit ElementSort(QXmlStreamReader *aInputStream);

        void load(QXmlStreamReader *aInputStream);
        void save(QXmlStreamWriter *aOutputStream) const;

        QColor color() const;
        void setColor(const QColor &aColor);

    private:
        QColor mColor;
        //    signals:

        //    public slots:
};

#endif // ELEMENTSORT_H
