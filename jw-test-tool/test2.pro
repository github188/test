#-------------------------------------------------
#
# Project created by QtCreator 2014-07-10T13:51:51
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = test2
TEMPLATE = app


SOURCES += main.cpp\
        widget.cpp \
    sysinfo_form.cpp \
    main_form.cpp \
    warningform.cpp \
    jw_thread.cpp

HEADERS  += widget.h \
    sysinfo_form.h \
    main_form.h \
    warningform.h \
    jw_thread.h

FORMS    += widget.ui \
    sysinfo_form.ui \
    main_form.ui \
    warningform.ui

RESOURCES +=
