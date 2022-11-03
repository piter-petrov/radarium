#-------------------------------------------------
#
# Project created by QtCreator 2022-10-31T23:45:07
#
#-------------------------------------------------

QT       += core network

QT       -= gui

TARGET = LiveService
CONFIG   += console
CONFIG   -= app_bundle
TEMPLATE = app

QMAKE_CXXFLAGS += -std=c++14

PRECOMPILED_HEADER = stable.h

SOURCES += main.cpp \
    liveserver.cpp \
    storage.cpp \
    point.cpp \
    log.cpp \
    broadcastpacketbuilder.cpp \
    broadcaster.cpp

HEADERS += \
    stable.h \
    liveserver.h \
    storage.h \
    point.h \
    log.h \
    broadcastpacketbuilder.h \
    broadcaster.h \
    safefifo.h
