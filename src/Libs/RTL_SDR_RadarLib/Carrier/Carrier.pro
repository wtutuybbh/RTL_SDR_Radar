#-------------------------------------------------
#
# Project created by QtCreator 2018-07-04T11:04:01
#
#-------------------------------------------------

QT       += gui

TARGET = Carrier
TEMPLATE = lib

DEFINES += CARRIER_LIBRARY

SOURCES += Carrier.cpp \
    NullCarrier.cpp \
    ServiceLocator.cpp \
    NavigationSystem.cpp \
    NullNavigationSystem.cpp

HEADERS += ../../../include/Carrier/carrier_global.h \
    ../../../include/Carrier/Carrier.h\
    ../../../include/Carrier/NullCarrier.h \
    ../../../include/Carrier/ServiceLocator.h \
    ../../../include/interface/ICarrierClass.h \
    ../../../include/interface/INavigationSystem.h \
    NavigationSystem.h \
    NullNavigationSystem.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}

include( ../../../../common.pri )
include( ../../../../lib.pri )

