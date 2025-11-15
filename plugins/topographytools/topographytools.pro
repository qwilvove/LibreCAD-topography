#-------------------------------------------------
#
# Project created by QtCreator 2011-03-22T19:33:11
#
#-------------------------------------------------

QT       += widgets
TEMPLATE = lib
CONFIG += plugin
VERSION = 0.0.1
TARGET = $$qtLibraryTarget(topographytools)

GENERATED_DIR = ../../generated/plugin/topographytools
# Use common project definitions.
include(../../common.pri)

# For plugins
INCLUDEPATH    += ../../librecad/src/plugins

SOURCES += \
    src/topographytools.cpp \
    src/class/tt_classpoint.cpp \
    src/ui/tt_dialogadd.cpp \
    src/ui/tt_dialogdrawblocks.cpp \
    src/ui/tt_dialogedit.cpp \
    src/ui/tt_dialoggrid.cpp \
    src/ui/tt_dialogimport.cpp \
    src/ui/tt_dialogmain.cpp \
    src/ui/tt_dialogpoints.cpp \
    src/ui/tt_dialogpolygo.cpp \
    src/ui/tt_dialogv0.cpp

HEADERS += \
    src/topographytools.h \
    src/tt.h \
    src/class/tt_classpoint.h \
    src/ui/tt_dialogadd.h \
    src/ui/tt_dialogdrawblocks.h \
    src/ui/tt_dialogedit.h \
    src/ui/tt_dialoggrid.h \
    src/ui/tt_dialogimport.h \
    src/ui/tt_dialogmain.h \
    src/ui/tt_dialogpoints.h \
    src/ui/tt_dialogpolygo.h \
    src/ui/tt_dialogv0.h

# Installation Directory
win32 {
        DESTDIR = ../../windows/resources/plugins
}
unix {
    macx {
        DESTDIR = ../../LibreCAD.app/Contents/Resources/plugins
    }
    else {
        DESTDIR = ../../unix/resources/plugins
    }
}

FORMS += \
    src/ui/tt_dialogadd.ui \
    src/ui/tt_dialogdrawblocks.ui \
    src/ui/tt_dialogedit.ui \
    src/ui/tt_dialoggrid.ui \
    src/ui/tt_dialogimport.ui \
    src/ui/tt_dialogmain.ui \
    src/ui/tt_dialogpoints.ui \
    src/ui/tt_dialogpolygo.ui \
    src/ui/tt_dialogv0.ui

RESOURCES += \
    res/blocks/blocks.qrc \
    res/icons/icons.qrc \
    res/thumbnails/thumbnails.qrc
