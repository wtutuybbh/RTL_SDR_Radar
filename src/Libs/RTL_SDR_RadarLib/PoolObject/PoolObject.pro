#-------------------------------------------------
#
# Project created by QtCreator 2018-04-16T17:21:54
#
#-------------------------------------------------

QT       += gui

TARGET = PoolObject
TEMPLATE = lib

DEFINES += POOLOBJECT_LIBRARY

SOURCES += PoolObject.cpp \
    factory/FactoryObjects.cpp \
    ../shared_classes/objects/base/BaseObject.cpp \
    ../shared_classes/objects/air/Aircraft.cpp

HEADERS += ../../../include/PoolObject/PoolObject.h\
    ../../../include/PoolObject/poolobject_global.h \
    ../../../include/interface/IObject.h \
    ../../../include/interface/ILogger.h \
    ../../../include/interface/IPoolObject.h \
    ../../../include/PoolObject/factory/FactoryObjects.h \
    ../shared_classes/objects/base/BaseObject.h \
    ../shared_classes/objects/air/Aircraft.h \
    ../shared_classes/objects/air/StructAircraft.h
unix {
    target.path = /usr/lib
    INSTALLS += target
}


include( ../../../../common.pri )
include( ../../../../lib.pri )
