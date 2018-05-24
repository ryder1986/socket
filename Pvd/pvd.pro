TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt


INCLUDEPATH +=cppjson/include
SOURCES += main.cpp tool.cpp server.cpp cppjson/json_reader.cpp cppjson/json_writer.cpp  cppjson/json_value.cpp \
    app.cpp \
    configmanager.cpp pvd.cpp videosource.cpp
HEADERS+=tool.h server.h \
    app.h \
    configmanager.h pvd.h videosource.h
DEFINES+=IS_UNIX
LIBS+=-lpthread

install_files.files+=res
install_files.path=$$OUT_PWD/
INSTALLS+=install_files

DISTFILES += \
    res/config.json
LIBS+=-lopencv_core -lopencv_highgui \
-lopencv_objdetect -lopencv_imgproc -lopencv_ml -lopencv_highgui \
 -lopencv_video
LIBS+=-lavcodec -lavformat -lavutil -lswresample
