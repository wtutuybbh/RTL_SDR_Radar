#-------------------------------------------------
#
# Project created by QtCreator 2018-04-16T17:21:54
#
#-------------------------------------------------

QT       += gui widgets

TARGET = PoolObject
TEMPLATE = lib

DEFINES += POOLOBJECT_LIBRARY

SOURCES += PoolObject.cpp \
    ../../../include/coord/Position.cpp \
    ../../../include/coord/Conversions.cpp \
    factory/FactoryObjects.cpp \
    ../../../include/objects/base/BaseObject.cpp \
    ../../../include/objects/base/GraphicsObject.cpp \
    ../../../include/objects/air/Aircraft.cpp

HEADERS += PoolObject.h\
        poolobject_global.h \
    ../../../include/interface/IObject.h \
    ../../../include/interface/ILogger.h \
    ../../../include/interface/IPoolObject.h \
    ../../../include/coord/Position.h \
    ../../../include/coord/Conversions.h \
    factory/FactoryObjects.h \
    ../../../include/objects/base/BaseObject.h \
    ../../../include/objects/base/GraphicsObject.h \
    ../../../include/objects/air/Aircraft.h \
    ../../../include/objects/air/StructAircraft.h
unix {
    target.path = /usr/lib
    INSTALLS += target
}


include( ../../../../common.pri )
include( ../../../../lib.pri )
