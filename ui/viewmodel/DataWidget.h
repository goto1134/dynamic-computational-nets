#ifndef DATAWIDGET_H
#define DATAWIDGET_H

#include <QObject>
#include <QLabel>
#include <QHBoxLayout>

class DataWidget : public QWidget
{
        Q_OBJECT

    public:
        explicit DataWidget(const QString &aText, QWidget *aValueWidget,QWidget *parent = 0);
    private:
        QWidget *mValueWidget;
        QHBoxLayout *mLayout;

    signals:

    public slots:
};

#endif // DATAWIDGET_H
