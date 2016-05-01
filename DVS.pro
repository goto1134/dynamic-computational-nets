#-------------------------------------------------
#
# Project created by QtCreator 2016-04-11T00:03:23
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = DVS
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    elementsort.cpp

HEADERS  += mainwindow.h \
    elementsort.h

FORMS    += mainwindow.ui

RESOURCES += \
    resources.qrc

DISTFILES += \
    icons/new project.ico \
    translations/translation_en.ts \
    translations/translation_ru.ts

TRANSLATIONS += translations/translation_en.ts \
    translations/translation_ru.ts
