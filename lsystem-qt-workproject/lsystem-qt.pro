# -------------------------------------------------
# Project created by QtCreator 2010-05-12T16:08:43
# -------------------------------------------------
QT -= gui
TARGET = lsystem-qt
CONFIG += console
CONFIG -= app_bundle
TEMPLATE = app
DEFINES += QT_APP
SOURCES += main.cpp \
    ../Garden/StringUtils.cpp \
    ../Garden/Configuration.cpp \
    ../Garden/LSFileGrammar.cpp \
    ../Garden/fparser/fpoptimizer.cc \
    ../Garden/fparser/fparser.cc
HEADERS += ../Garden/LSFileGrammar.h \
    ../Garden/AbstractGrammar.h \
    ../Garden/StaticString.h \
    ../Garden/Rule.h \
    ../Garden/Configuration.h \
    ../Garden/StringUtils.h \
    precompiled.h
INCLUDEPATH += ../Garden/ \
    ../Garden/fparser
