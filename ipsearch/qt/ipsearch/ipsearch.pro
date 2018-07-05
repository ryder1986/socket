TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

HEADERS += \
    ../../zenarp.h \
    ../../zenlog.h \
    ../../zenplat.h

SOURCES += \
    ../../catcharp.c \
    ../../main.c \
    ../../sendarp.c \
    ../../zenlog.c

DISTFILES += \
    ../../Makefile



