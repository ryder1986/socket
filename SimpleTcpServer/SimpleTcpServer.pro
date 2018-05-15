TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.cpp \
    server.cpp tool.cpp

HEADERS += \
    server.h tool.h

LIBS+=-lpthread
DEFINES+=IS_UNIX
