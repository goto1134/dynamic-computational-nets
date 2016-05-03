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
    ui/viewmodel/ProjectTreeModel.cpp \
    ui/viewmodel/TreeItem.cpp \
    ui/MainWindow.cpp \
    model/ElementSort.cpp \
    model/Transition.cpp

HEADERS  += \
    ui/viewmodel/ProjectTreeModel.h \
    ui/viewmodel/TreeItem.h \
    ui/MainWindow.h \
    model/ElementSort.h \
    model/Transition.h

FORMS    += \
    ui/mainwindow.ui

RESOURCES += \
    resources.qrc

DISTFILES += \
    icons/new project.ico \
    translations/translation_en.ts \
    translations/translation_ru.ts \
    global/sorts.xml

TRANSLATIONS += translations/translation_en.ts \
    translations/translation_ru.ts
