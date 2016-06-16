#-------------------------------------------------
#
# Project created by QtCreator 2016-04-11T00:03:23
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = DNC
TEMPLATE = app

SOURCES += main.cpp\
    model/base/ProjectObject.cpp \
    model/base/ProjectGraphicsObject.cpp \
    model/base/ProjectNamedObject.cpp \
    model/ElementSort.cpp \
    model/NetClass.cpp \
    model/ObjectNet.cpp \
    model/Place.cpp \
    model/Connection.cpp \
    model/TerminalTransition.cpp \
    model/NonTerminalTransition.cpp \
    model/ProjectModel.cpp \
    ui/viewmodel/ProjectTreeModel.cpp \
    ui/viewmodel/TreeItem.cpp \
    ui/MainWindow.cpp \
    ui/redactor/ObjectNetRedactor.cpp \
    ui/redactor/items/ArrowItem.cpp \
    ui/redactor/items/NetObjectItem.cpp \
    ui/viewmodel/DataWidget.cpp \
    ui/viewmodel/PropertyWidget.cpp

HEADERS  += \
    model/base/ProjectObject.h \
    model/base/ProjectGraphicsObject.h \
    model/base/ProjectNamedObject.h \
    model/Place.h \
    model/Connection.h \
    model/TerminalTransition.h \
    model/NonTerminalTransition.h \
    model/ProjectModel.h \
    model/ElementSort.h \
    model/NetClass.h \
    model/ObjectNet.h \
    ui/viewmodel/ProjectTreeModel.h \
    ui/viewmodel/TreeItem.h \
    ui/viewmodel/PropertyWidget.h \
    ui/viewmodel/DataWidget.h \
    ui/redactor/items/NetObjectItem.h \
    ui/redactor/items/ArrowItem.h \
    ui/redactor/ObjectNetRedactor.h \
    ui/MainWindow.h

FORMS    += \
    ui/mainwindow.ui

RESOURCES += \
    resources.qrc

DISTFILES += icons/new project.ico \
    translations/translation_en.ts \
    translations/translation_ru.ts \
    DCN.rc

RC_FILE = DCN.rc

TRANSLATIONS += translations/translation_en.ts \
    translations/translation_ru.ts
