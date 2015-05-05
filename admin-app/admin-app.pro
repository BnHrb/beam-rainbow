#-------------------------------------------------
#
# Project created by QtCreator 2015-04-29T13:29:26
#
#-------------------------------------------------

QT       += core gui
QT       += sql
QT       += webkitwidgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = admin-app
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    questiondialog.cpp \
    helpdialog.cpp \
    qcustomplot.cpp

HEADERS  += mainwindow.h \
    questiondialog.h \
    helpdialog.h \
    qcustomplot.h

FORMS    += mainwindow.ui \
    questiondialog.ui \
    helpdialog.ui

DISTFILES +=
