TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt


INCLUDEPATH +=cppjson/include
SOURCES += main.cpp tool.cpp server.cpp cppjson/json_reader.cpp cppjson/json_writer.cpp  cppjson/json_value.cpp \
    app.cpp \
    configmanager.cpp pvd.cpp videosource.cpp test.cpp c4common.cpp c4processor.cpp \
    pvdobject.cpp hogprocessor.cpp
HEADERS+=tool.h server.h \
    app.h \
    configmanager.h pvd.h videosource.h test.h  c4common.h \
    pvdobject.h c4processor.h  hogprocessor.h
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
INCLUDEPATH+=track
SOURCES +=track/Ctracker.cpp track/HungarianAlg.cpp  track/Kalman.cpp
HEADERS += track/defines.h track/HungarianAlg.h track/Kalman.h track/Ctracker.h
