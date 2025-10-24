# Project name
TARGET = archconv+

# Required Qt modules
QT += core

# Source files
SOURCES += main.cpp
HEADERS += grubtty.h
HEADERS += kdegrub.h
HEADERS += gnomegrub.h
HEADERS += ttysystemd-boot.h
HEADERS += kdesystemd-boot.h
# C++ standard
CONFIG += c++23
