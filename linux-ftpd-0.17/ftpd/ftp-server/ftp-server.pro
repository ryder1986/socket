TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES +=\
../ftpcmd.c  ../ftpd.c  ../logutmp.c  ../logwtmp.c  ../popen.c ../../support/vis.c  ../../support/setproctitle.c
HEADERS+=\
../extern.h  ../pathnames.h ../../support/daemon.h ../../support/vis.h ../../support/setproctitle.h
INCLUDEPATH+=../../support
LIBS+=-lcrypt
