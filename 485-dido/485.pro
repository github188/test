#-------------------------------------------------
#
# Project created by QtCreator 2014-10-22T10:44:48
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = 485
TEMPLATE = app

LIBS +=./libtdwy_io_board_api.a

SOURCES += main.cpp\
        widget.cpp \
    tdwy_io_board_api.c

HEADERS  += widget.h \
    tdwy_io_board_api.h

FORMS    += widget.ui
