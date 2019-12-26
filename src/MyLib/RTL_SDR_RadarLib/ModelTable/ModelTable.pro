#-------------------------------------------------
#
# Project created by QtCreator 2018-05-03T15:14:33
#
#-------------------------------------------------

QT       += widgets

TARGET = ModelTable
TEMPLATE = lib

DEFINES += MODELTABLERTO_LIBRARY

SOURCES += \
    ModelTable.cpp \
    ../../../include/coord/Conversions.cpp

HEADERS +=\
    ModelTable.h \
    modeltable_global.h \
    ../../../include/interface/IPoolObject.h \
    ../../../include/interface/IObserver.h \
    ../../../include/interface/IObject.h \
    ../../../include/coord/Conversions.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}

include( ../../../../common.pri )
include( ../../../../lib.pri )

FORMS +=
