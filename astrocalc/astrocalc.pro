#-------------------------------------------------
#
# Project created by QtCreator 2011-11-09T15:00:28
#
#-------------------------------------------------

TARGET = astrocalc
TEMPLATE = lib
CONFIG += staticlib

SOURCES += astrocalc.cpp

HEADERS += astrocalc.h
unix:!symbian {
    maemo5 {
        target.path = /opt/usr/lib
    } else {
        target.path = /usr/lib
    }
    INSTALLS += target
}


