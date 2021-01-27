#-------------------------------------------------
#
# Project created by QtCreator 2018-11-22T11:00:31
#
#-------------------------------------------------
include( ../../../../common.pri )
include( ../../../../lib.pri )

QT       += gui network widgets

TARGET = DataController
TEMPLATE = lib

DEFINES += DATACONTROLLER_LIBRARY

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    DataController.cpp \
    DataWorker.cpp \
    network/DataWorkerNetSender.cpp \
    network/NetworkWorkerTcp.cpp \
    network/NetworkWorkerUdp.cpp \
    protocol/ProtocolPkgProcessor.cpp

HEADERS += \
        ../../../include/interface/INetworkWorker.h \
        ../../../include/DataController/DataController.h \
    network/DataWorkerNetSender.h \
        ../../../include/DataController/datacontroller_global.h \
    ../../../include/interface/IDataController.h \
    ../../../include/interface/IWorker.h \
    ../../../include/interface/IDemodulator.h \
    DataWorker.h \
    ../../../include/interface/IDSP.h \
    network/NetworkWorkerTcp.h \
    network/NetworkWorkerUdp.h \
    protocol/ProtocolPkgProcessor.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}

LIBS += -lCarrier
