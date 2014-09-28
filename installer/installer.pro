QT += widgets

#LIBS *= -L/home/lxfb/pig_deploy/lib -lQt5Widgets -lQt5Gui -lQt5Core  # TODO: CAMBIAR EL PATH RELATIVO
QMAKE_LFLAGS += -Wl,--rpath=./lib 

TARGET = installer
TEMPLATE = app

SOURCES += src/main.cpp\
           src/installer.cpp

HEADERS += src/installer.h

RESOURCES += \
           resources.qrc
