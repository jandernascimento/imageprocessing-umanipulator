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
    dialogkernel3.cpp \
    dialogkernel4.cpp \
    dialogkernel5.cpp \
    dialoglog.cpp \
    dialogabout.cpp \
    dialogsetkernel.cpp

HEADERS  += mainwindow.h \
    custom/imageabstration.h \
    custom/imagelabel.h \
    dialogcontrast.h \
    histogram.h \
    dialogfusion.h \
    dialogcustom.h \
    dialogscale.h \
    dialogkernel3.h \
    dialogkernel4.h \
    dialogkernel5.h \
    dialoglog.h \
    dialogabout.h \
    dialogsetkernel.h


FORMS    += mainwindow.ui \
    dialogcontrast.ui \
    histogram.ui \
    dialogfusion.ui \
    dialogcustom.ui \
    dialogscale.ui \
    dialogkernel3.ui \
    dialogkernel4.ui \
    dialogkernel5.ui \
    dialoglog.ui \
    dialogabout.ui \
    dialogsetkernel.ui


RESOURCES += \
    resource.qrc
