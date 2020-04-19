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
    ../shared_classes/coord/Conversions.cpp \
    ModelTable.cpp

HEADERS +=\
    ../../../include/ModelTable/ModelTable.h \
    ../../../include/ModelTable/modeltable_global.h \
    ../../../include/interface/IObserver.h \
    ../../../include/interface/IPoolObject.h \
    ../../../include/interface/IObject.h \
    ../shared_classes/coord/Conversions.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}

include( ../../../../common.pri )
include( ../../../../lib.pri )

FORMS +=
