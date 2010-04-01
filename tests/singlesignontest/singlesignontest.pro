include( ../../common-project-config.pri )

include( ../../common-vars.pri )

CONFIG += qtestlib

QT += core \
     dbus

QT -= gui

LIBPATH += ../../src/libsignon-qt
LIBS += -lsignon-qt

SOURCES = testsinglesignon.cpp


HEADERS += ../../include/signoncommon.h \
	   ../../src/libsignon-qt/authinfo.h \
	   ../../src/libsignon-qt/authservice.h \
	   ../../src/libsignon-qt/identity.h


TARGET = testsinglesignon

INCLUDEPATH += . \
    ../../include \
    ../../src/libsignon-qt 
