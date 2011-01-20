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
    dialogfusion.cpp \
    dialogcustom.cpp \
    dialogscale.cpp \
    dialoglog.cpp \
    dialogabout.cpp \
    dialogsetkernel.cpp \
    intelligentresizing.cpp

HEADERS  += mainwindow.h \
    custom/imageabstration.h \
    custom/imagelabel.h \
    dialogcontrast.h \
    histogram.h \
    dialogfusion.h \
    dialogcustom.h \
    dialogscale.h \
    dialoglog.h \
    dialogabout.h \
    dialogsetkernel.h


FORMS    += mainwindow.ui \
    dialogcontrast.ui \
    histogram.ui \
    dialogfusion.ui \
    dialogcustom.ui \
    dialogscale.ui \
    dialoglog.ui \
    dialogabout.ui \
    dialogsetkernel.ui


RESOURCES += \
    resource.qrc
