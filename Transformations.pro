QT += core gui

CONFIG += c++11

TARGET = Transformations
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp \
    vertex.cpp \
    window.cpp \
    transform3d.cpp

HEADERS += window.h \
    vertex.h \
    transform3d.h

RESOURCES += \
    resources.qrc
