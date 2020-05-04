#-------------------------------------------------
#
# Project created by QtCreator 2018-09-19T16:24:37
#
#-------------------------------------------------

QT       += core gui widgets opengl network printsupport

TARGET = RadarApp
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

DEFINES += QCUSTOMPLOT_USE_OPENGL

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

include( ../../../common.pri )
include( ../../../app.pri )

SOURCES += \
    ../../../import/osm/coordUtils/Conversions.cpp \
    ../../../import/osm/coordUtils/Position.cpp \
    ../../../import/qcustomplot/qcustomplot.cpp \
        main.cpp \
        gui/MainWindow.cpp \
    AppCore/Core.cpp \
    AppCore/publisher/Subject.cpp \
    gui/TableForm.cpp

HEADERS += \
    ../../../import/osm/coordUtils/Position.h \
    ../../../import/osm/coordUtils/ScreenConversions.h \
    ../../../import/qcustomplot/qcustomplot.h \
        gui/MainWindow.h \
    AppCore/Core.h \
    AppCore/publisher/Subject.h \
    gui/TableForm.h

FORMS += \
        gui/MainWindow.ui \
    gui/TableForm.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    ../../../import/res.qrc

DISTFILES += \
    ../../../import/style.qss

LIBS += -lLogger \
        -lPoolObject \
        -lGraphicsWidget \
        -lCarrier \
        -lDataController \
        -lRTL_SDR_Reciver \
        -lDemodulator \
        -lModelTable \
        -lDSPLib
