#-------------------------------------------------
#
# Project created by QtCreator 2015-12-28T19:29:14
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = platform_qt
TEMPLATE = app


SOURCES += \
    main.cpp \
    mainwindow.cpp \
    ../common/common_main.c \
    ../common/LockFreeFIFO.c \
    ../common/SlotBasedMenu.c \
    ../common/StackBasedFsm.c

HEADERS  += \
    main.h \
    mainwindow.h \
    ../common/common_main.h \
    ../common/config.h \
    ../common/LockFreeFIFO.h \
    ../common/SlotBasedMenu.h \
    ../common/StackBasedFsm.h
