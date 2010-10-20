#-------------------------------------------------
#
# Project created by QtCreator 2010-07-22T09:49:23
#
#-------------------------------------------------

QT       += core

QT       -= gui

TARGET = lsystem-qt-2
CONFIG   += console
CONFIG   += exceptions
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += main.cpp \
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
    ../boost_1_43_0/libs/program_options/src/cmdline.cpp \
    ../Garden/fparser/fparser.cc \
    ../Garden/fparser/fpoptimizer.cc \
    ../Garden/LSFileGrammar.cpp \
    ../Garden/lsfile.cpp \
    ../Garden/configuration.cpp \
    ../Garden/longstring.cpp \
    ../Garden/log.cpp \
    ../Garden/grammargenerator.cpp \
    ../Garden/utils.cpp \
    ../Garden/lsystemgrammar.cpp \
    ../Garden/d0lsystemgrammar.cpp \
    ../Garden/abstract0lsystemgrammar.cpp \
    ../Garden/parstoch0lsystemgrammar.cpp \
    ../Garden/pard0lsystemgrammar.cpp \
    ../Garden/abstractklsystemgrammar.cpp

HEADERS += \
    ../Garden/staticstring.h \
    ../Garden/rule.h \
    ../Garden/lsystemexception.h \
    ../Garden/LSFileGrammar.h \
    ../Garden/lsfile.h \
    ../Garden/abstractgrammar.h \
    ../Garden/abstractfile.h \
    ../Garden/longstring.h \
    ../Garden/log.h \
    ../Garden/abstractgenerator.h \
    ../Garden/grammargenerator.h \
    ../Garden/parseablestring.h \
    ../Garden/utils.h \
    ../Garden/configuration.h \
    ../Garden/randomindex.h \
    ../Garden/lsystemgrammar.h \
    ../Garden/d0lsystemgrammar.h \
    ../Garden/abstract0lsystemgrammar.h \
    ../Garden/parstoch0lsystemgrammar.h \
    ../Garden/abstractklsystemgrammar.h \
    ../Garden/pard0lsystemgrammar.h

INCLUDEPATH += ../Garden/ \
    ../Garden/fparser \
    ../boost_1_43_0

DEFINES += QT_APP
