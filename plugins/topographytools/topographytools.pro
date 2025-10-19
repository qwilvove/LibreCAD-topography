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
    topographytools.cpp \
    tt_classpoint.cpp \
    tt_dialogadd.cpp \
    tt_dialogdrawblocks.cpp \
    tt_dialogedit.cpp \
    tt_dialoggrid.cpp \
    tt_dialogimport.cpp \
    tt_dialogmain.cpp \
    tt_dialogpoints.cpp \
    tt_dialogpolygo.cpp \
    tt_dialogv0.cpp

HEADERS += \
    topographytools.h \
    tt.h \
    tt_classpoint.h \
    tt_dialogadd.h \
    tt_dialogdrawblocks.h \
    tt_dialogedit.h \
    tt_dialoggrid.h \
    tt_dialogimport.h \
    tt_dialogmain.h \
    tt_dialogpoints.h \
    tt_dialogpolygo.h \
    tt_dialogv0.h

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
    tt_dialogadd.ui \
    tt_dialogdrawblocks.ui \
    tt_dialogedit.ui \
    tt_dialoggrid.ui \
    tt_dialogimport.ui \
    tt_dialogmain.ui \
    tt_dialogpoints.ui \
    tt_dialogpolygo.ui \
    tt_dialogv0.ui

RESOURCES += \
    blocks/blocks.qrc \
    icons/icons.qrc \
    thumbnails/thumbnails.qrc
