# -------------------------------------------------
# Project created by QtCreator 2010-05-12T16:08:43
# -------------------------------------------------
QT -= gui
QT += core
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
    ../Garden/fparser/fparser.cc \
    ../Garden/lsfile.cpp \
    ../boost_1_43_0/libs/program_options/src/winmain.cpp \
    ../boost_1_43_0/libs/program_options/src/variables_map.cpp \
    ../boost_1_43_0/libs/program_options/src/value_semantic.cpp \
    ../boost_1_43_0/libs/program_options/src/utf8_codecvt_facet.cpp \
    ../boost_1_43_0/libs/program_options/src/split.cpp \
    ../boost_1_43_0/libs/program_options/src/positional_options.cpp \
    ../boost_1_43_0/libs/program_options/src/parsers.cpp \
    ../boost_1_43_0/libs/program_options/src/options_description.cpp \
    ../boost_1_43_0/libs/program_options/src/convert.cpp \
    ../boost_1_43_0/libs/program_options/src/config_file.cpp \
    ../boost_1_43_0/libs/program_options/src/cmdline.cpp
HEADERS += ../Garden/LSFileGrammar.h \
    ../Garden/AbstractGrammar.h \
    ../Garden/StaticString.h \
    ../Garden/Rule.h \
    ../Garden/Configuration.h \
    ../Garden/StringUtils.h \
    precompiled.h \
    ../Garden/abstractfile.h \
    ../Garden/lsfile.h \
    ../Garden/lsystemexception.h
INCLUDEPATH += ../Garden/ \
    ../Garden/fparser \
    ../boost_1_43_0
