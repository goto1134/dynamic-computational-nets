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
    model/NetClass.cpp \
    model/ObjectNet.cpp \
    ui/redactor/ObjectNetRedactor.cpp \
    ui/redactor/items/ArrowItem.cpp \
    ui/redactor/items/NetObjectItem.cpp \
    ui/viewmodel/objectwithproperties.cpp \
    model/ProjectObject.cpp \
    model/ProjectNamedObject.cpp \
    model/Place.cpp \
    model/Connection.cpp \
    model/TerminalTransition.cpp \
    model/NonTerminalTransition.cpp

HEADERS  += \
    ui/viewmodel/ProjectTreeModel.h \
    ui/viewmodel/TreeItem.h \
    ui/MainWindow.h \
    model/ElementSort.h \
    model/NetClass.h \
    model/ObjectNet.h \
    ui/redactor/ObjectNetRedactor.h \
    ui/redactor/items/ArrowItem.h \
    ui/redactor/items/NetObjectItem.h \
    ui/viewmodel/ObjectWithProperties.h \
    model/ProjectObject.h \
    model/ProjectNamedObject.h \
    model/Place.h \
    model/Connection.h \
    model/TerminalTransition.h \
    model/NonTerminalTransition.h

FORMS    += \
    ui/mainwindow.ui

RESOURCES += \
    resources.qrc

DISTFILES += icons/new project.ico \
    translations/translation_en.ts \
    translations/translation_ru.ts

TRANSLATIONS += translations/translation_en.ts \
    translations/translation_ru.ts
