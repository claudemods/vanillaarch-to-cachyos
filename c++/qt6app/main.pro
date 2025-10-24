# Project name
TARGET = archconv+

# Required Qt modules
QT += core widgets concurrent

# Source files
SOURCES += main.cpp
HEADERS += grubtty.h
HEADERS += kdegrub.h
HEADERS += gnomegrub.h
HEADERS += ttysystemd-boot.h
HEADERS += kdesystemd-boot.h
HEADERS += gnomesystemd-boot.h
# C++ standard
CONFIG += c++23
