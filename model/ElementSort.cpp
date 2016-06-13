#include "ElementSort.h"

#include <QString>
#include <QVariant>
#include <QDebug>
#include <QRgb>

const QString ELEMENT_SORT_LABEL = "sort";
const QString COLOR_LABEL = "color";
const QString R_LABEL = "r";
const QString G_LABEL = "g";
const QString B_LABEL = "b";


ElementSort::ElementSort(const QString &aName, const QColor &aColor)
    :ProjectNamedObject(ProjectObject::Sort, aName)
{
    qDebug() << "Sort " << aName;
    mColor = aColor;
}

ElementSort::ElementSort(QXmlStreamReader *aInputStream)
{
    setType(ProjectObject::Sort);
    load(aInputStream);
}


void ElementSort::load(QXmlStreamReader *aInputStream)
{
    if (aInputStream->isStartElement()
            && aInputStream->name() == ELEMENT_SORT_LABEL)
    {
        if(aInputStream->readNextStartElement()
                && aInputStream->name() == COLOR_LABEL)
        {
            ushort r,g,b;
            foreach (QXmlStreamAttribute attribute, aInputStream->attributes())
            {
                if(attribute.name() == R_LABEL)
                {
                    r = attribute.value().toUShort();
                }
                else if(attribute.name() == G_LABEL)
                {
                    g = attribute.value().toUShort();
                }
                else if(attribute.name() == B_LABEL)
                {
                    b = attribute.value().toUShort();
                }
            }
            aInputStream->skipCurrentElement();
            mColor = QColor(r,g,b);
        }
    }
    aInputStream->readNextStartElement();
    ProjectNamedObject::load(aInputStream);
    aInputStream->skipCurrentElement();
}

void ElementSort::save(QXmlStreamWriter *aOutputStream) const
{
    aOutputStream->writeStartElement(ELEMENT_SORT_LABEL);
    {
        aOutputStream->writeStartElement(COLOR_LABEL);
        {
            aOutputStream->writeAttribute(R_LABEL, QString::number(mColor.red()));
            aOutputStream->writeAttribute(G_LABEL, QString::number(mColor.green()));
            aOutputStream->writeAttribute(B_LABEL, QString::number(mColor.blue()));
        }
        aOutputStream->writeEndElement();
        ProjectNamedObject::save(aOutputStream);
    }
    aOutputStream->writeEndElement();
}

QColor ElementSort::color() const
{
    return mColor;
}

void ElementSort::setColor(const QColor &aColor)
{
    mColor = aColor;
}
