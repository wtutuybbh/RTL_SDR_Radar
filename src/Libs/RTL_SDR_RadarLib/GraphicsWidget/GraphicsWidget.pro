#-------------------------------------------------
#
# Project created by QtCreator 2018-04-20T16:41:19
#
#-------------------------------------------------

QT       += widgets opengl core gui network

TARGET = GraphicsWidget
TEMPLATE = lib

DEFINES += GRAPHICSWIDGET_LIBRARY

SOURCES += GraphicsWidget.cpp \
    ../shared_classes/coord/ScreenConversions.cpp \
    implements/MapController.cpp \
    ../../../../import/osm/MapGraphicsNetwork.cpp \
    ../../../../import/osm/MapTileSource.cpp \
    ../../../../import/osm/OSMTileSource.cpp \
    objects/GraphicsObject.cpp

HEADERS += ../../../include/GraphicsWidget/GraphicsWidget.h\
    ../../../include/GraphicsWidget/graphicswidget_global.h \
    ../../../include/interface/IMapController.h \
    ../shared_classes/coord/ScreenConversions.h \
    implements/MapController.h \
    ../../../../import/osm/MapGraphicsNetwork.h \
    ../../../../import/osm/MapTileSource.h \
    ../../../../import/osm/OSMTileSource.h \
    ../../../include/interface/IObserver.h \
    ../../../include/interface/ISubject.h \
    objects/GraphicsObject.h \
    ../../../include/interface/IObject.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}

include( ../../../../common.pri )
include( ../../../../lib.pri )


LIBS += -lCarrier
