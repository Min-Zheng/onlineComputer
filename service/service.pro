TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.cpp \
    matrix.cpp
LIBS += -lpthread -lboost_system -lboost_thread

HEADERS += \
    matrix.h
