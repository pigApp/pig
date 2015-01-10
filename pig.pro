TARGET = pig
TEMPLATE = app

DEFINES += BOOST_ASIO_DYN_LINK
win32 {
    DEFINES += WIN32_LEAN_AND_MEAN
    load(moc)
    QMAKE_MOC = $$QMAKE_MOC -DBOOST_TT_HAS_OPERATOR_HPP_INCLUDED
}

QT += qml quick sql network widgets 

SOURCES += src/main.cpp\
           src/pig.cpp\
           src/password.cpp\
           src/update.cpp\
           src/tcpSocket.cpp\
           src/torrent.cpp\
           src/unzip.cpp \
           src/su.cpp

HEADERS += src/pig.h\
           src/password.h\
           src/update.h\
           src/tcpSocket.h\
           src/torrent.h\
           src/unzip.h \
           src/su.h

RESOURCES = resources.qrc

RC_FILE = resources/images/global/icon.rc

unix {
    CONFIG += link_pkgconfig
    QMAKE_RPATHDIR += /usr/lib/pig
    LIBS += -L/usr/lib/pig -ltorrent-rasterbar -lboost_system -lquazip -lz
    INCLUDEPATH += /usr/include/pig

    target.path = /usr/bin
    config.path = $$(HOME)/.pig
    config.files = .pig/db.sqlite
    config.extra = mkdir $$(HOME)/.pig -m 744 -p
    permission.path = $$(HOME)/.pig
    permission.extra = chown $$(USER).users $$(HOME)/.pig -R
    INSTALLS += target config permission
}

win32 {
    LIBS += -LC:/boost/lib -lpsapi\
            -LC:/libtorrent-rasterbar-1.0.2/bin/msvc-12.0/release/address-model-64/architecture-x86/boost-link-shared/boost-source/resolve-countries-off/threading-multi -ltorrent
    INCLUDEPATH += C:/boost/include/boost-1_56\
                   C:/libtorrent-rasterbar-1.0.2/include

    target.path = C:\PIG
    target.extra = mkdir C:\PIG -m 744 -p #TODO: CONFIRMAR SI FUNCIONA
    config.path = C:\PIG\.pig
    config.files = .pig\db.sqlite
    config.extra = mkdir C:\PIG\.pig -m 744 -p
    update.path = C:\PIG\.pig
    update.files = src\scripts\update\win32\update.bat
    INSTALLS += target config update
}
