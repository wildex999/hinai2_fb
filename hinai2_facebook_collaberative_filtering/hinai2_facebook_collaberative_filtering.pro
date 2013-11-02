#-------------------------------------------------
#
# Project created by QtCreator 2013-10-31T15:45:16
#
#-------------------------------------------------

QT       += core gui network

TARGET = hinai2_facebook_collaberative_filtering
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    fbgraph_parser.cpp \
    person.cpp \
    product.cpp \
    post.cpp \
    comment.cpp \
    networkmanager.cpp \
    collaberativefiltering.cpp \
    like.cpp

HEADERS  += mainwindow.h \
    fbgraph_parser.h \
    person.h \
    product.h \
    post.h \
    comment.h \
    networkmanager.h \
    collaberativefiltering.h \
    like.h

FORMS    += mainwindow.ui

LIBS += -lqjson
