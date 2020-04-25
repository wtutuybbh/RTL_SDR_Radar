#-------------------------------------------------
#
# Project created by QtCreator 2018-04-02T14:34:41
#
#-------------------------------------------------

QT       -= gui

TARGET = Logger
TEMPLATE = lib

DEFINES += LOGGER_LIBRARY

include( ../../../../common.pri )
include( ../../../../lib.pri )

SOURCES += \
    Logger.cpp

HEADERS +=\
        ../../../include/Logger/logger_global.h \
        ../../../include/Logger/Logger.h \
        ../../../include/interface/ILogger.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}

