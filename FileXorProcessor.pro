QT += widgets

CONFIG += c++17 console
CONFIG -= app_bundle

TEMPLATE = app

TARGET = FileXorProcessor

SOURCES += \
    src/main.cpp \
    src/mainwindow.cpp \
    src/fileprocessor.cpp

HEADERS += \
    src/mainwindow.h \
    src/fileprocessor.h

