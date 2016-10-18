#-------------------------------------------------
#
# Project created by QtCreator 2015-12-28T17:18:01
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = qt_project
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    ../../common/common_main.c \
    ../../common/LockFreeFIFO.c \
    ../../common/SlotBasedMenu.c \
    ../../common/StackBasedFsm.c

HEADERS  += mainwindow.h \
    ../../common/config.h \
    main.h \
    ../../common/common_main.h \
    ../../common/LockFreeFIFO.h \
    ../../common/SlotBasedMenu.h \
    ../../common/StackBasedFsm.h \
    ../../hal/hal_display.h \
    ../../hal/hal_filesystem.h \
    ../../hal/hal_inputdevice.h \
    ../../hal/hal_mididevice.h \
    ../../hal/hal_misc.h

FORMS +=
