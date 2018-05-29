HEADERS += \
    testtimer.h \
    testprocess.h \
    test.h videosource.h videoprocessor.h\
    c4processor.h c4common.h \
    testsource.h cameramanager.h app.h\
    camera.h configmanager.h hogprocessor.h

SOURCES += \
    testtimer.cpp tool.cpp \
    main.cpp cppjson/json_reader.cpp cppjson/json_writer.cpp  cppjson/json_value.cpp \
    testprocess.cpp \
    test.cpp videosource.cpp videoprocessor.cpp\
    c4processor.cpp c4common.cpp \
    testsource.cpp cameramanager.cpp app.cpp\
    camera.cpp configmanager.cpp  hogprocessor.cpp

CONFIG +=c++11
DEFINES +=IS_UNIX
INCLUDEPATH +=cppjson/include

LIBS+=-lopencv_core -lopencv_highgui \
-lopencv_objdetect -lopencv_imgproc -lopencv_ml -lopencv_highgui \
 -lopencv_video
LIBS+=-lavcodec -lavformat -lavutil -lswresample


INCLUDEPATH+=track
SOURCES +=track/Ctracker.cpp track/HungarianAlg.cpp  track/Kalman.cpp
HEADERS += track/defines.h track/HungarianAlg.h track/Kalman.h track/Ctracker.h


install_files.files+=res
install_files.path=$$OUT_PWD/
INSTALLS +=install_files
