TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt


INCLUDEPATH +=cppjson/include
SOURCES += main.cpp tool.cpp server.cpp cppjson/json_reader.cpp cppjson/json_writer.cpp  cppjson/json_value.cpp \
    app.cpp \
    configmanager.cpp pvd.cpp
HEADERS+=tool.h server.h \
    app.h \
    configmanager.h pvd.h
DEFINES+=IS_UNIX
LIBS+=-lpthread

install_files.files+=res
install_files.path=$$OUT_PWD/
INSTALLS+=install_files

DISTFILES += \
    res/config.json
