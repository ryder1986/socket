TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt
#QMAKE_LFLAGS+= -lpthread

HEADERS += \
    ../../zenarp.h \
    ../../zenlog.h \
    ../../zenplat.h \
    ../../arp.h

SOURCES += \
    ../../catcharp.c \
    ../../main.c \
    ../../sendarp.c \
    ../../zenlog.c \
    ../../arp.c

QMAKE_LFLAGS +=-pthread


QMAKE_CXXFLAGS+=-w
