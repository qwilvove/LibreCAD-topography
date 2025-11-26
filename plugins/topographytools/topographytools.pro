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
    src/class/plugin_settings.cpp \
    src/class/point.cpp \
    src/class/project_settings.cpp \
    src/static/io.cpp \
    src/topographytools.cpp \
    src/ui/dialog/add.cpp \
    src/ui/dialog/draw_blocks.cpp \
    src/ui/dialog/edit.cpp \
    src/ui/dialog/grid.cpp \
    src/ui/dialog/import.cpp \
    src/ui/dialog/main.cpp \
    src/ui/dialog/points.cpp \
    src/ui/dialog/polygo.cpp \
    src/ui/dialog/tt_ui_dialog_draw_points.cpp \
    src/ui/dialog/tt_ui_dialog_plugin_settings.cpp \
    src/ui/dialog/tt_ui_dialog_project_settings.cpp \
    src/ui/dialog/v0.cpp

HEADERS += \
    src/class/plugin_settings.h \
    src/class/point.h \
    src/class/project_settings.h \
    src/static/io.h \
    src/topographytools.h \
    src/tt.h \
    src/ui/dialog/add.h \
    src/ui/dialog/draw_blocks.h \
    src/ui/dialog/edit.h \
    src/ui/dialog/grid.h \
    src/ui/dialog/import.h \
    src/ui/dialog/main.h \
    src/ui/dialog/points.h \
    src/ui/dialog/polygo.h \
    src/ui/dialog/tt_ui_dialog_draw_points.h \
    src/ui/dialog/tt_ui_dialog_plugin_settings.h \
    src/ui/dialog/tt_ui_dialog_project_settings.h \
    src/ui/dialog/v0.h

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
    src/ui/dialog/add.ui \
    src/ui/dialog/draw_blocks.ui \
    src/ui/dialog/edit.ui \
    src/ui/dialog/grid.ui \
    src/ui/dialog/import.ui \
    src/ui/dialog/main.ui \
    src/ui/dialog/points.ui \
    src/ui/dialog/polygo.ui \
    src/ui/dialog/tt_ui_dialog_draw_points.ui \
    src/ui/dialog/tt_ui_dialog_plugin_settings.ui \
    src/ui/dialog/tt_ui_dialog_project_settings.ui \
    src/ui/dialog/v0.ui

RESOURCES += \
    res/blocks/blocks.qrc \
    res/icons/icons.qrc \
    res/thumbnails/thumbnails.qrc
