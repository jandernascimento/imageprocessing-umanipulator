#-------------------------------------------------
#
# Project created by QtCreator 2011-01-04T17:49:13
#
#-------------------------------------------------

QT       += core gui

TARGET = umanipulator
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    mainwindowactions.cpp \
    custom/imageabstration.cpp \
    custom/imagelabel.cpp \
    dialogcontrast.cpp \
    histogram.cpp \
    dialogcustom.cpp \
    dialogfusion.cpp

HEADERS  += mainwindow.h \
    custom/imageabstration.h \
    custom/imagelabel.h \
    dialogcontrast.h \
    histogram.h \
    dialogcustom.h \
    dialogfusion.h

FORMS    += mainwindow.ui \
    dialogcontrast.ui \
    histogram.ui \
    custom.ui \
    dialogcustom.ui \
    dialogfusion.ui

