QT       += widgets opengl core gui network

TEMPLATE = lib
DEFINES += MAPGRAPHICSWIDGET_LIBRARY

include( ../../../../common.pri )
include( ../../../../lib.pri )


# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    ../../../../import/MapGraphics/MapGraphicsObject.cpp \
    ../../../../import/MapGraphics/MapGraphicsScene.cpp \
    ../../../../import/MapGraphics/MapGraphicsView.cpp \
    ../../../../import/MapGraphics/MapTileSource.cpp \
    ../../../../import/MapGraphics/coordUtils/Conversions.cpp \
    ../../../../import/MapGraphics/coordUtils/Position.cpp \
    ../../../../import/MapGraphics/coordUtils/ScreenConversions.cpp \
    ../../../../import/MapGraphics/graphBackend/MapGraphicsNetwork.cpp \
    ../../../../import/MapGraphics/graphBackend/MapTileGraphicsObject.cpp \
    ../../../../import/MapGraphics/graphBackend/PrivateQGraphicsInfoSource.cpp \
    ../../../../import/MapGraphics/graphBackend/PrivateQGraphicsObject.cpp \
    ../../../../import/MapGraphics/graphBackend/PrivateQGraphicsScene.cpp \
    ../../../../import/MapGraphics/graphBackend/PrivateQGraphicsView.cpp \
    ../../../../import/MapGraphics/graphObjects/CircleObject.cpp \
    ../../../../import/MapGraphics/graphObjects/LineObject.cpp \
    ../../../../import/MapGraphics/graphObjects/PolygonObject.cpp \
    ../../../../import/MapGraphics/tileSources/CompositeTileSource.cpp \
    ../../../../import/MapGraphics/tileSources/GridTileSource.cpp \
    ../../../../import/MapGraphics/tileSources/OSMTileSource.cpp \
    MapGraphicsWidget.cpp

HEADERS += \
    ../../../../import/MapGraphics/MapGraphicsObject.h \
    ../../../../import/MapGraphics/MapGraphicsScene.h \
    ../../../../import/MapGraphics/MapGraphicsView.h \
    ../../../../import/MapGraphics/MapGraphics_global.h \
    ../../../../import/MapGraphics/MapTileSource.h \
    ../../../../import/MapGraphics/coordUtils/Conversions.h \
    ../../../../import/MapGraphics/coordUtils/Position.h \
    ../../../../import/MapGraphics/coordUtils/ScreenConversions.h \
    ../../../../import/MapGraphics/graphBackend/MapGraphicsNetwork.h \
    ../../../../import/MapGraphics/graphBackend/MapTileGraphicsObject.h \
    ../../../../import/MapGraphics/graphBackend/PrivateQGraphicsInfoSource.h \
    ../../../../import/MapGraphics/graphBackend/PrivateQGraphicsObject.h \
    ../../../../import/MapGraphics/graphBackend/PrivateQGraphicsScene.h \
    ../../../../import/MapGraphics/graphBackend/PrivateQGraphicsView.h \
    ../../../../import/MapGraphics/graphObjects/CircleObject.h \
    ../../../../import/MapGraphics/graphObjects/LineObject.h \
    ../../../../import/MapGraphics/graphObjects/PolygonObject.h \
    ../../../../import/MapGraphics/tileSources/CompositeTileSource.h \
    ../../../../import/MapGraphics/tileSources/GridTileSource.h \
    ../../../../import/MapGraphics/tileSources/OSMTileSource.h \
    ../../../include/MapGraphicsWidget/MapGraphicsWidget_global.h \
    ../../../include/MapGraphicsWidget/MapGraphicsWidget.h

# Default rules for deployment.
unix {
    target.path = /usr/lib
}
!isEmpty(target.path): INSTALLS += target
