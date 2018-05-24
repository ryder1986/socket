HEADERS += \
    testtimer.h \
    testprocess.h \
    test.h

SOURCES += \
    testtimer.cpp tool.cpp \
    main.cpp cppjson/json_reader.cpp cppjson/json_writer.cpp  cppjson/json_value.cpp \
    testprocess.cpp \
    test.cpp

CONFIG +=c++11
DEFINES +=IS_UNIX
INCLUDEPATH +=cppjson/include

