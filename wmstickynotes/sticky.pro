TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
    wmstickynotes.c

HEADERS += \
    wmstickynotes.h

INCLUDEPATH+=/usr/include/gtk-2.0/ /usr/include/glib-2.0/ /usr/lib/i386-linux-gnu/glib-2.0/include /usr/include/gtk-2.0/gdk/ /usr/include/cairo/ \
/usr/include/pango-1.0/ /usr/lib/i386-linux-gnu/gtk-2.0/include /usr/include/gdk-pixbuf-2.0/ /usr/include/atk-1.0/
LIBS+=-lgtk-x11-2.0  -lX11 -lgobject-2.0 -lgdk-x11-2.0 -lglib-2.0
