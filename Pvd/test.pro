HEADERS += \
    testtimer.h

SOURCES += \
    testtimer.cpp tool.cpp \
    main.cpp cppjson/json_reader.cpp cppjson/json_writer.cpp  cppjson/json_value.cpp

CONFIG +=c++11
DEFINES +=IS_UNIX
INCLUDEPATH +=cppjson/include

