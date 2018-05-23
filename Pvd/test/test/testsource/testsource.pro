TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.cpp

HEADERS+=../../../videosource.h ../../../tool.h
SOURCES+=../../../videosource.cpp ../../../tool.cpp
INCLUDEPATH +=../../../
DEFINES+=IS_UNIX

#LIBS+=-L/home/zen/opencv/lib -lopencv_core -lopencv_highgui \
#-lopencv_objdetect -lopencv_imgproc -lopencv_ml -lopencv_highgui -lopencv_videoio \
# -lopencv_video -lpthread
#INCLUDEPATH+=/home/zen/opencv//include
LIBS+= -lopencv_video -lpthread -lopencv_core -lopencv_highgui -lopencv_objdetect -lopencv_imgproc -lopencv_ml -lopencv_highgui
