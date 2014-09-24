QT += core gui network widgets

TARGET = installer
TEMPLATE = app

SOURCES += src/main.cpp\
           src/installer.cpp \
           src/tcpSocket.cpp

HEADERS += src/installer.h \
           src/tcpSocket.h

RESOURCES += \
           resources.qrc
